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
 * @brief Manage the generation and use of nonces (numbers used once)
 * @section DESCRIPTION
 *
 * The Nonce class is used for managing nonces. It supports assignment (e.g. 
 * where a nonce is sent from the Pico to the server) and ganeration using
 * OpenSSL's secure random number generator.
 *
 */

/** \addtogroup Crypto
 *  @{
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include "pico/debug.h"
#include "pico/log.h"
#include "pico/nonce.h"

// Defines

// Structure definitions

/**
 * @brief Generate and handle numbers that are only used once (nonces)
 *
 * Opaque structure containing the private fields of the Nonce class.
 * 
 * This is provided as the first parameter of every non-static function and 
 * stores the operation's context.
 * 
 * The structure typedef is in nonce.h
 */
struct _Nonce {
	unsigned char * value;
	size_t length;
};

// Function prototypes

// Function definitions

/**
 * Create a new instance of the class.
 *
 * @return The newly created object.
 */
Nonce * nonce_new() {
	Nonce * nonce;

	nonce = CALLOC(sizeof(Nonce), 1);
	
	return nonce;
}

/**
 * Delete an instance of the class, freeing up the memory allocated to it.
 *
 * @param nonce The object to free.
 */
void nonce_delete(Nonce * nonce) {
	if (nonce) {
		if (nonce->value) {
			FREE(nonce->value);
		}
		FREE(nonce);
	}
}

/**
 * Set the value of a nonce. The buffer can be safely deleted afterwards,
 * since the Nonce object stores a copy of the data.
 *
 * @param nonce The nonce to set
 * @param value A buffer containing the value of the nonce. This should be a sequence
 *              bytes. It can be any length, but the standard is 8 bytes
 */
void nonce_set_buffer(Nonce * nonce, Buffer * value) {
	size_t length;
	length = buffer_get_pos(value);

	if (nonce->value) {
		FREE(nonce->value);
	}

	nonce->length = length;	
	nonce->value = MALLOC(length);
	memcpy(nonce->value, buffer_get_buffer(value), length);
}

/**
 * Set the value of a nonce to a randomly generated value. OpenSSL's secure
 * random functions are used to generate 8 bytes of data, equivalent to an
 * unsigned value between 0 and 2^64 = 18446744073709551616.
 *
 * @param nonce The nonce to set
 */
void nonce_generate_random(Nonce * nonce) {
	int result;

	if (nonce->value) {
		FREE(nonce->value);
	}
	nonce->value = MALLOC(NONCE_DEFAULT_BYTES);

	result = RAND_bytes(nonce->value, NONCE_DEFAULT_BYTES);	
	if (result != 1) {
		LOG(LOG_ERR,"Error (randomness): %lu\n", ERR_get_error());
	}
	nonce->length = NONCE_DEFAULT_BYTES;
}

/**
 * Returns a pointer to the start of the buffer containing the nonce value.
 * Use nonce_get_length() to determine how many bytes are stored in the buffer.
 *
 * @param nonce The Nonce object to get the buffer of
 * @return Pointer to the start of the buffer
 */
unsigned char const * nonce_get_buffer(Nonce * nonce) {
	return nonce->value;
}

/**
 * Returns the length of data stored in the Nonce object.
 *
 * @param nonce The Nonce object to query
 * @return The number of bytes of data stored in the nonce
 */
size_t nonce_get_length(Nonce * nonce) {
	return nonce->length;
}

/** @} addtogroup Crypto */

