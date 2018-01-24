/** \ingroup Message
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
 * @brief Message for continuously authenticating the verifier to the prover
 * @section DESCRIPTION
 *
 * The MessageServiceReAuth class allows a Status message to ge generated by
 * the server to be sent to the Pico.
 * 
 * This represents a message sent as part of the continuous authentication
 * process.
 * QR-code (KeyAuth or KeyPair); Start; ServiceAuth; PicoAuth; Status;
 * Pico ReAuth; Service ReAuth (repeatedly).
 *
 * The structure of the message is as follows
 * {"iv":"B64","encryptedData":"B64-ENC","sessionId":0}
 * Where encryptedData contains the following sections
 * char reauthState | int timeout | length | char sequenceNumber[length]
 *
 */

/** \addtogroup Message
 *  @{
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "pico/debug.h"
#include "pico/cryptosupport.h"
#include "pico/json.h"
#include "pico/buffer.h"
#include "pico/base64.h"
#include "pico/log.h"
#include "pico/sequencenumber.h"
#include "pico/messageservicereauth.h"

// Defines

// Structure definitions

/**
 * @brief Structure for storing service re-authentication message details
 *
 * Opaque structure containing the private fields of the MessageServiceReAuth
 * class.
 *
 * This is provided as the first parameter of every non-static function and 
 * stores the operation's context.
 * 
 * The structure typedef is in messageservicereauth.h
 */
struct _MessageServiceReAuth {
	Buffer * sharedKey;
	int sessionId;

	SequenceNumber * sequenceNum;
	REAUTHSTATE reauthState;
	long int timeout; // 4-byte signed integer value

	Buffer * iv;
	Buffer * encryptedData;
};

// Function prototypes

// Function definitions

/**
 * Create a new instance of the class.
 *
 * @return The newly created object.
 */
MessageServiceReAuth * messageservicereauth_new() {
	MessageServiceReAuth * messageservicereauth;

	messageservicereauth = CALLOC(sizeof(MessageServiceReAuth), 1);
	messageservicereauth->sharedKey = buffer_new(0);

	messageservicereauth->sessionId = 0;

	messageservicereauth->sequenceNum = sequencenumber_new();
	messageservicereauth->reauthState = REAUTHSTATE_INVALID;
	messageservicereauth->timeout = -1;

	messageservicereauth->iv = buffer_new(0);
	messageservicereauth->encryptedData = buffer_new(0);

	return messageservicereauth;
}

/**
 * Delete an instance of the class, freeing up the memory allocated to it.
 *
 * @param messageservicereauth The object to free.
 */
void messageservicereauth_delete(MessageServiceReAuth * messageservicereauth) {
	if (messageservicereauth) {
		if (messageservicereauth->sharedKey) {
			buffer_delete(messageservicereauth->sharedKey);
		}

		if (messageservicereauth->sequenceNum) {
			sequencenumber_delete(messageservicereauth->sequenceNum);
		}

		if (messageservicereauth->iv) {
			buffer_delete(messageservicereauth->iv);
		}
		if (messageservicereauth->encryptedData) {
			buffer_delete(messageservicereauth->encryptedData);
		}

		FREE(messageservicereauth);
	}
}

/**
 * Populate a Status message structure with the data it needs to be used.
 *
 * @param messageservicereauth The MessageServiceReAuth object to initalise
 * @param sharedKey Object containing the data shared between all messages during
 *                  the protocol run 
 * @param timeout The timeout to send to the Pico. If the Pico doesn't respond
 *        correctly within this time the service should consider
 *        authentication to have failed.
 * @param reauthState The re-authentication state to send to the Pico. See
 *        the REAUTHSTATE enum in messagepicoreauth.h for the possible values
 *        this can take.
 * @param sequenceNum The sequence number to use for the service.
 */
void messageservicereauth_set(MessageServiceReAuth * messageservicereauth, Buffer * sharedKey, long int timeout, REAUTHSTATE reauthState, SequenceNumber const * sequenceNum) {
	buffer_clear(messageservicereauth->sharedKey);
	buffer_append_buffer(messageservicereauth->sharedKey, sharedKey);

	messageservicereauth->reauthState = reauthState;
	messageservicereauth->timeout = timeout;
	if (sequenceNum) {
		sequencenumber_copy(messageservicereauth->sequenceNum, sequenceNum);
	}
}

/**
 * Serialize the Status data in JSON format.
 *
 * @param messageservicereauth The object for serialization.
 * @param buffer Memory buffer to store the result in.
 */
void messageservicereauth_serialize(MessageServiceReAuth * messageservicereauth, Buffer * buffer) {
	Json * json;
	Buffer * encrypted;
	Buffer * encoded;
	Buffer * iv;
	Buffer * toEncrypt;
	char reauthState;
	char timeout[4];

	json = json_new();

	// Encrypted data
	toEncrypt = buffer_new(1);

	// Reauth State
	reauthState = (char)messageservicereauth->reauthState;
	buffer_append(toEncrypt, & reauthState, 1);

	// Timeout
	timeout[0] = (messageservicereauth->timeout >> 24) & 0xff;
	timeout[1] = (messageservicereauth->timeout >> 16) & 0xff;
	timeout[2] = (messageservicereauth->timeout >> 8) & 0xff;
	timeout[3] = (messageservicereauth->timeout >> 0) & 0xff;
	buffer_append(toEncrypt, timeout, 4);

	// Sequence number
	buffer_append_lengthprepend(toEncrypt, sequencenumber_get_raw_bytes(messageservicereauth->sequenceNum), SEQUENCE_NUMBER_LENGTH);

	iv = buffer_new(CRYPTOSUPPORT_IV_SIZE);
	cryptosupport_generate_iv(iv);

	encrypted = buffer_new(0);
	//sharedKey = shared_get_shared_key(messageservicereauth->shared);
	cryptosupport_encrypt(messageservicereauth->sharedKey, iv, toEncrypt, encrypted);

	json_add_integer(json, "sessionId", messageservicereauth->sessionId);

	encoded = buffer_new(0);
	base64_encode_buffer(encrypted, encoded);
	json_add_buffer(json, "encryptedData", encoded);

	buffer_clear(encoded);
	base64_encode_buffer(iv, encoded);
	json_add_buffer(json, "iv", encoded);

	json_serialize_buffer(json, buffer);
	json_delete(json);
	
	buffer_delete(toEncrypt);
	buffer_delete(encrypted);
	buffer_delete(encoded);
	buffer_delete(iv);
}

/**
 * Deserialize a ServiceAuth JSON string (likely received by the protocol from
 * a Pico) and store the data collected from it into a MessageServiceReAuth
 * object.
 *
 * The function will return false if the deserialization fails. Reasons
 * for failure include:
 *  - A malformed JSON string
 *  - Failure for the encrypted data section to decrypt properly (the data is
 *    encrypted using GCM, which includes a MAC and so incorrect decryption
 *    will be identified)
 *  - An incorrect MAC
 *  - An invalid signature
 *  - A zero length or NULL buffer.
 *
 * @param messageservicereauth The MessageServiceReAuth object to store the
 *        deserialized data into.
 * @param buffer The JSON string to deserialize.
 * @return true if the message was deserialized correctly, false o/w.
 */
bool messageservicereauth_deserialize(MessageServiceReAuth * messageservicereauth, Buffer const * buffer) {
	Json * json;
	char const * value;
	Buffer * cleartext;
	size_t start;
	size_t next;
	bool result;
	int timeout;
	unsigned char const * timeout_buffer;

	// The structure of the message is as follows
	// {"iv":"B64","encryptedData":"B64-ENC","sessionId":0}
	// Where encryptedData contains the following sections
	// char reauthState | int timeout | length | char sequenceNumber[length]

	Buffer * bytes;
	size_t length;

	json = json_new();
	result = json_deserialize_buffer(json, buffer);

	if (result) {
		if (json_get_type(json, "sessionId") == JSONTYPE_INTEGER) {
			messageservicereauth->sessionId = json_get_integer(json, "sessionId");
		}
		else {
			LOG(LOG_ERR, "Missing sessionId\n");
			result = false;
		}
	}

	if (result) {
		value = json_get_string(json, "iv");
		if (value) {
			base64_decode_string(value, messageservicereauth->iv);
		}
		else {
			LOG(LOG_ERR, "Missing iv\n");
			result = false;
		}
	}

	if (result) {
		value = json_get_string(json, "encryptedData");
		if (value) {
			base64_decode_string(value, messageservicereauth->encryptedData);
		}
		else {
			LOG(LOG_ERR, "Missing encryptedData\n");
			result = false;
		}
	}

	cleartext = buffer_new(0);
	if (result) {
	//sharedKey = shared_get_shared_key(messagepicoreauth->shared);
		result = cryptosupport_decrypt(messageservicereauth->sharedKey, messageservicereauth->iv, messageservicereauth->encryptedData, cleartext);
	}

	bytes = buffer_new(0);

	if (result) {
		// char reauthState | int timeout | length | char sequenceNumber[length]
		length = buffer_get_pos(cleartext);
		start = 0;

		// char reauthState
		next = start + sizeof(char);
		if ((next > start) && (next < length)) {
			messageservicereauth->reauthState = (REAUTHSTATE)buffer_get_buffer(cleartext)[0];
			start = next;
		}
		else {
			LOG(LOG_ERR, "Error deserializing decrypted reauth state data\n");
			result = false;
		}

		// int timeout
		next = start + sizeof(int);
		if ((next > start) && (next < length)) {
			timeout_buffer = (unsigned char *) buffer_get_buffer(cleartext);
			timeout = 0;
			timeout |= (timeout_buffer[start + 0] << 24);
			timeout |= (timeout_buffer[start + 1] << 16);
			timeout |= (timeout_buffer[start + 2] << 8);
			timeout |= (timeout_buffer[start + 3] << 0);
			messageservicereauth->timeout = timeout;
			start = next;
		}
		else {
			LOG(LOG_ERR, "Error deserializing decrypted reauth timeout\n");
			result = false;
		}

		// length | char sequenceNumber[length]
		buffer_clear(bytes);
		next = buffer_copy_lengthprepend(cleartext, start, bytes);
		length = buffer_get_pos(bytes);
		if ((next > start) && (length == SEQUENCE_NUMBER_LENGTH)) {
			sequencenumber_transfer_from_buffer(messageservicereauth->sequenceNum, bytes);
			start = next;
		}
		else {
			LOG(LOG_ERR, "Error deserializing decrypted length-prepended challenge sequence number data\n");
			result = false;
		}
	}
	
	if (result) {
		// If we got here successfully, it is expected to have consumed the whole buffer
		result = start == buffer_get_pos(cleartext);
	}

	buffer_delete(bytes);
	buffer_delete(cleartext);

	json_delete(json);

	return result;
}

/**
 * Returns the timeout sent by the service
 * This number basically means that the service is expecting Pico to send a 
 * MessagePicoReAuth before the specified amount of time
 *
 * @param messageservicereauth The MessageServiceReAuth object
 * @return The timeout in milliseconds
 */
int messageservicereauth_get_timeout(MessageServiceReAuth * messageservicereauth) {
	return messageservicereauth->timeout;
}

/**
 * Return the sequence number to be sent by the Service as part of the
 * MessageServiceReAuth.
 *
 * @param messageservicereauth The MessageServiceReAuth object to get the
 *        sequence number from.
 * @param sequenceNum Allocated object to store the sequence number in.
 */
void messageservicereauth_get_sequencenum(MessageServiceReAuth * messageservicereauth, SequenceNumber * sequenceNum) {
	if (sequenceNum) {
		sequencenumber_copy(sequenceNum, messageservicereauth->sequenceNum);
	}
}

/**
 * Return the reauth state sent by the Service as part of the
 * MessageServiceReAuth.
 *
 * @param messageservicereauth The MessageServiceReAuth object to get the state.
 * @return The reauth state.
 */
REAUTHSTATE messageservicereauth_get_reauthstate(MessageServiceReAuth * messageservicereauth) {
	return messageservicereauth->reauthState;
}

/**
 * Set the reauth state to be sent by the Service as part of the
 * MessageServiceReAuth.
 *
 * @param messageservicereauth The MessageServiceReAuth object to set the state
 *        of.
 * @param reauthstate the state to set the message to.
 */
void messageservicereauth_set_reauthstate(MessageServiceReAuth * messageservicereauth, REAUTHSTATE reauthstate) {
	messageservicereauth->reauthState = reauthstate;
}

/** @} addtogroup Message */

