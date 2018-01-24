/** \ingroup Crypto
 * @file
 * @author  David Llewellyn-Jones <David.Llewellyn-Jones@cl.cam.ac.uk>
 * @version $(VERSION)
 *
 * @section LICENSE
 *
 * (C) Copyright Cambridge Authentication Ltd, 2017
 *
 * This file is part of libpico.
 *
 * Libpico is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Libpico is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with libpico. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 * @brief Generate shared secrets from ECDH keys
 * @section DESCRIPTION
 *
 * The SharedSigmaKeyDerivclass takes a ECDH-generates shared secret and uses
 * it to generate a bundle more shared secrets for use in other parts of the
 * Sigma-I protocol (for the ECDH generation approach, see the KeyAgreement
 * class).
 *
 * The approach is based on the key derivation protocol used in Internet Key 
 * Exchange (IKE), as specified in RFC 4306 and NIST SP 800-135
 * http://tools.ietf.org/search/rfc4306#section-2.13
 * csrc.nist.gov/publications/nistpubs/800-135-rev1/sp800-135-rev1.pdf
 * 
 * In the randomness extraction step, the key derivation key is derived by 
 * taking the SHA256 HMAC of the shared secret obtained using a prior
 * public-key-based key agreement procedure, where the key to the HMAC is
 * $N_P || N_S$ where $N_P$ is the nonce sent by the Pico and $N_S$ is the 
 * nonce sent by the service.
 * 
 * In the key expansion step, each block of key material is derived using a 
 * SHA256 HMAC. The key to the HMAC is the key deriving key, and the message 
 * for block $k$, $B_k$$, is $B_{k - 1} || k || N_P || N_S$, except for the 
 * first block, where the previous block component is ommitted.
 * 
 */

/** \addtogroup Crypto
 *  @{
 */

#include <stdio.h>
#include <openssl/hmac.h>
#include <malloc.h>
#include "pico/debug.h"
#include "pico/buffer.h"
#include "pico/base64.h"
#include "pico/log.h"
#include "pico/sigmakeyderiv.h"

// Defines

#define SIGMAKEYDERIV_BLOCK_SIZE_IN_BYTES (32)
#define SIGMAKEYDERIV_START_BLOCK_NUMBER (1)
#define SIGMAKEYDERIV_MAX_BLOCK_NUMBER (255)

// Structure definitions

/**
 * @brief Generate shared secrets from ECDH keys
 *
 * Opaque structure containing the private fields of the SigmaKeyDeriv class.
 * 
 * This is provided as the first parameter of every non-static function and 
 * stores the operation's context.
 * 
 * The structure typedef is in sigmakeyderiv.h
 */
struct _SigmaKeyDeriv {
	int blockSizeInBytes;
	//Mac keyExpansionMac;
	Buffer * currentBlock;
	int numCurrentBlockBytesUsed;

	Buffer * sharedSecret;
	Buffer * nonces;
	int currentBlockNumber;

	HMAC_CTX * ctx;
};

// Function prototypes

void sigmakeyderiv_extract_randomness(SigmaKeyDeriv * sigmakeyderiv, Buffer * bufferout);
void sigmakeyderiv_next_block(SigmaKeyDeriv * sigmakeyderiv);

// Function definitions

/**
 * Create a new instance of the class.
 *
 * @return The newly created object.
 */
SigmaKeyDeriv * sigmakeyderiv_new() {
	SigmaKeyDeriv * sigmakeyderiv;

	sigmakeyderiv = CALLOC(sizeof(SigmaKeyDeriv), 1);

	sigmakeyderiv->blockSizeInBytes = SIGMAKEYDERIV_BLOCK_SIZE_IN_BYTES;
	sigmakeyderiv->currentBlock = buffer_new(sigmakeyderiv->blockSizeInBytes);
	sigmakeyderiv->sharedSecret = buffer_new(0);
	sigmakeyderiv->nonces = buffer_new(0);
	sigmakeyderiv->ctx = CALLOC(sizeof(HMAC_CTX), 1);

	return sigmakeyderiv;
}

/**
 * Delete an instance of the class, freeing up the memory allocated to it.
 *
 * @param sigmakeyderiv The object to free.
 */
void sigmakeyderiv_delete(SigmaKeyDeriv * sigmakeyderiv) {
	if (sigmakeyderiv) {
		if (sigmakeyderiv->ctx) {
			HMAC_CTX_cleanup(sigmakeyderiv->ctx);
			FREE(sigmakeyderiv->ctx);
		}
		if (sigmakeyderiv->currentBlock) {
			buffer_delete(sigmakeyderiv->currentBlock);
		}
		if (sigmakeyderiv->sharedSecret) {
			buffer_delete(sigmakeyderiv->sharedSecret);
		}
		if (sigmakeyderiv->nonces) {
			buffer_delete(sigmakeyderiv->nonces);
		}

		FREE(sigmakeyderiv);
	}
}

/**
 * Populate the SigmaKeyDeriv object with the data needed for it to generate
 * the new set of shared secrets.
 *
 * @param sigmakeyderiv Object to set up
 * @param sharedSecret Buffer containing the shared secret generated using the
 *        service's ephemeral private key and the Pico's ephemeral public key.
 *        This can be generated using keyagreement_generate_secret().
 * @param picoNonce The nonce received from the Pico in the Start message
 * @param serviceNonce The nonce generated by the service and sent to the Pico
 *        in the ServiceAuth message
 */
void sigmakeyderiv_set(SigmaKeyDeriv * sigmakeyderiv, Buffer * sharedSecret, Nonce * picoNonce, Nonce * serviceNonce) {
	Buffer * randomness;

	sigmakeyderiv->blockSizeInBytes = SIGMAKEYDERIV_BLOCK_SIZE_IN_BYTES;
	buffer_clear(sigmakeyderiv->currentBlock);
	//sigmakeyderiv->keyExpansionMac = sha256Hmac;
	sigmakeyderiv->numCurrentBlockBytesUsed = sigmakeyderiv->blockSizeInBytes;
	sigmakeyderiv->currentBlockNumber = SIGMAKEYDERIV_START_BLOCK_NUMBER;

	buffer_clear(sigmakeyderiv->sharedSecret);
	buffer_append_buffer(sigmakeyderiv->sharedSecret, sharedSecret);
	
	buffer_clear(sigmakeyderiv->nonces);
	buffer_append(sigmakeyderiv->nonces, nonce_get_buffer(picoNonce), nonce_get_length(picoNonce));
	buffer_append(sigmakeyderiv->nonces, nonce_get_buffer(serviceNonce), nonce_get_length(serviceNonce));

	randomness = buffer_new(EVP_MAX_MD_SIZE);
	sigmakeyderiv_extract_randomness(sigmakeyderiv, randomness);

	HMAC_CTX_init(sigmakeyderiv->ctx);
	HMAC_Init_ex(sigmakeyderiv->ctx, buffer_get_buffer(randomness), buffer_get_pos(randomness), EVP_sha256(), NULL);
	
	buffer_delete(randomness);
}

/**
 * Generate a 32-byte pseudo-random block of data using a seed that combines the
 * Pico's nonce, service's nonce and shared secret. This is actually a SHA256
 * HMAC of the Pico and Service nonces concatenated together, using the shared
 * secret as the HMAC key. Since all three vales are known to both the Pico and
 * the service, both are able to generate the same pseudo-random value. However,
 * since the shared secret will not be known to an attacker, and the nonces are
 * both fresh, an eavesdropper will not be able to generate this value.
 *
 * @param sigmakeyderiv The SigmaKeyDeriv object containing the data needed to
 *                      generate the HMAC
 * @param bufferout The buffer to write the 32-bytes of random data to
 */
void sigmakeyderiv_extract_randomness(SigmaKeyDeriv * sigmakeyderiv, Buffer * bufferout) {
	HMAC_CTX * ctx;
	unsigned int len;

	ctx = CALLOC(sizeof(HMAC_CTX), 1);

	HMAC_CTX_init(ctx);
	HMAC_Init_ex(ctx, buffer_get_buffer(sigmakeyderiv->nonces), buffer_get_pos(sigmakeyderiv->nonces), EVP_sha256(), NULL);
	HMAC_Update(ctx, (unsigned char * const)buffer_get_buffer(sigmakeyderiv->sharedSecret), buffer_get_pos(sigmakeyderiv->sharedSecret));

	buffer_set_min_size(bufferout, EVP_MAX_MD_SIZE);
	HMAC_Final(ctx, (unsigned char * const)buffer_get_buffer(bufferout), &len);
	buffer_set_pos(bufferout, len);

	HMAC_CTX_cleanup(ctx);

	FREE(ctx);
}

/**
 * Internal function for generating another block of pseudo-random data.
 * The block can be generated by both the Pico and the server, but not by an
 * attacker. Consequently the blocks can be used to generate shared keys, 
 * following the Sigma-I protocol.
 *
 * The result is stored in the sigmakeyderiv object and used by the
 * sigmakeyderiv_get_next_key() function.
 *
 * @param sigmakeyderiv The SigmaKeyDeriv object containing the data needed to
 *                      generate the next block, and where the result will
 *                      be stored
 */
void sigmakeyderiv_next_block(SigmaKeyDeriv * sigmakeyderiv) {
	Buffer * newBlockMacMessage;
	char currentBlockNumber;
	unsigned int len;

	if (sigmakeyderiv->currentBlockNumber > SIGMAKEYDERIV_MAX_BLOCK_NUMBER) {
		LOG(LOG_ERR, "Error generating shared key data: no more blocks\n");
	}

	newBlockMacMessage = buffer_new(0);

	if (sigmakeyderiv->currentBlockNumber == SIGMAKEYDERIV_START_BLOCK_NUMBER) {
		buffer_append_buffer(newBlockMacMessage, sigmakeyderiv->nonces);
		currentBlockNumber = (char)sigmakeyderiv->currentBlockNumber;
		buffer_append(newBlockMacMessage, & currentBlockNumber, sizeof(char));
	}
	else {
		buffer_append_buffer(newBlockMacMessage, sigmakeyderiv->currentBlock);
		buffer_append_buffer(newBlockMacMessage, sigmakeyderiv->nonces);
		currentBlockNumber = (char)sigmakeyderiv->currentBlockNumber;
		buffer_append(newBlockMacMessage, & currentBlockNumber, sizeof(char));
	}
	sigmakeyderiv->currentBlockNumber++;

	buffer_set_min_size(sigmakeyderiv->currentBlock, EVP_MAX_MD_SIZE);

	HMAC_Update(sigmakeyderiv->ctx, (unsigned char * const)buffer_get_buffer(newBlockMacMessage), buffer_get_pos(newBlockMacMessage));

	HMAC_Final(sigmakeyderiv->ctx, (unsigned char * const)buffer_get_buffer(sigmakeyderiv->currentBlock), &len);
	buffer_set_pos(sigmakeyderiv->currentBlock, len);

	HMAC_Init_ex(sigmakeyderiv->ctx, NULL, 0, NULL, NULL);

	buffer_delete(newBlockMacMessage);
}

/**
 * Internal function for generating a key shared between the Pico and the
 * service, but which can't be derived by an eavesdropper. The key can be
 * any length, and the function will call sigmakeyderiv_next_block() enough
 * times to generate the pseudo-randomness for the key.
 *
 * @param sigmakeyderiv The SigmaKeyDeriv object containing the data needed to
 *                      generate the key
 * @param keyBytes A buffer to store the resulting key in
 * @param length The length of key requested
 */
void sigmakeyderiv_get_next_key(SigmaKeyDeriv * sigmakeyderiv, Buffer * keyBytes, int length) {
	int numKeyBytesFilled;
	int numKeyBytes;
	int remaining;
	int available;
	int toCopy;
	
	numKeyBytes = length / 8;
	buffer_clear(keyBytes);
	buffer_set_min_size(keyBytes, numKeyBytes);

	// Fill keyBytes
	numKeyBytesFilled = 0;
	while (numKeyBytesFilled < numKeyBytes) {
		// Maybe do more expansion
		if (sigmakeyderiv->numCurrentBlockBytesUsed == sigmakeyderiv->blockSizeInBytes) {
			// Derive the next block of keying material
			sigmakeyderiv_next_block(sigmakeyderiv);
			sigmakeyderiv->numCurrentBlockBytesUsed = 0;
		}
		// Determine how many bytes to copy
		remaining = numKeyBytes - numKeyBytesFilled;
		available = sigmakeyderiv->blockSizeInBytes - sigmakeyderiv->numCurrentBlockBytesUsed;
		if (remaining > available) {
			toCopy = available;
		}
		else {
			toCopy = remaining;
		}
		// Copy the bytes from currentBlock to keyBytes
		buffer_append(keyBytes, buffer_get_buffer(sigmakeyderiv->currentBlock) + sigmakeyderiv->numCurrentBlockBytesUsed, toCopy);
		
		// Update variables
		sigmakeyderiv->numCurrentBlockBytesUsed += toCopy;
		numKeyBytesFilled += toCopy;
	}
	
	// Key to return is already in the output buffer
	// No working byte array to clean up
}

/** @} addtogroup Crypto */

