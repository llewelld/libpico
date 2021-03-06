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
 * @brief Message for finalising the SIGMA-I authentication protocol
 * @section DESCRIPTION
 *
 * The MessageStatus class allows a Status message to ge generated by
 * the server to be sent to the Pico.
 * 
 * This represents the second message forming the second round trip of the
 * Sigma-I protocol:
 * QR-code (KeyAuth or KeyPair); Start; ServiceAuth; PicoAuth; Status.
 *
 * The structure of the message is as follows
 * {"iv":"B64","encryptedData":"B64-ENC","sessionId":0}
 *
 */

/** \addtogroup Message
 *  @{
 */

#include <stdio.h>
#include <malloc.h>
#include "pico/debug.h"
#include "pico/cryptosupport.h"
#include "pico/json.h"
#include "pico/buffer.h"
#include "pico/base64.h"
#include "pico/log.h"
#include "pico/messagestatus.h"

// Defines

// Structure definitions

/**
 * @brief Structure for storing status message details
 *
 * Opaque structure containing the private fields of the MessageStatus class.
 *
 * This is provided as the first parameter of every non-static function and 
 * stores the operation's context.
 * 
 * The structure typedef is in messagestatus.h
 */
struct _MessageStatus {
	Shared * shared;
	int sessionId;
	char status;
	Buffer * extraData;
};

// Function prototypes

// Function definitions

/**
 * Create a new instance of the class.
 *
 * @return The newly created object.
 */
MessageStatus * messagestatus_new() {
	MessageStatus * messagestatus;

	messagestatus = CALLOC(sizeof(MessageStatus), 1);
	messagestatus->extraData = buffer_new(0);
	messagestatus->sessionId = 0;

	return messagestatus;
}

/**
 * Delete an instance of the class, freeing up the memory allocated to it.
 *
 * @param messagestatus The object to free.
 */
void messagestatus_delete(MessageStatus * messagestatus) {
	if (messagestatus) {
		if (messagestatus->extraData) {
			buffer_delete(messagestatus->extraData);
		}
		FREE(messagestatus);
	}
}

/**
 * Populate a Status message structure with the data it needs to be used.
 *
 * @param messagestatus The MessageStatus object to initalise
 * @param shared Object containing the data shared between all messages during
 *               the protocol run 
 * @param extraData string to be set as extra data
 */
void messagestatus_set(MessageStatus * messagestatus, Shared * shared, Buffer const * extraData, char status) {
	messagestatus->shared = shared;
	buffer_clear(messagestatus->extraData);
	if (extraData) {
		buffer_append_buffer(messagestatus->extraData, extraData);
	}
	messagestatus->status = status;
}

/**
 * Get the current status value recorded in the message. The possible values
 * are listed in messagestatus.h and are defined as follows.
 *
 * MESSAGESTATUS_OK_DONE ((char)0)
 * MESSAGESTATUS_OK_CONTINUE ((char)1)
 * MESSAGESTATUS_REJECTED ((char)-1)
 * MESSAGESTATUS_ERROR ((char)-2)
 *
 * @param messagestatus The MessageStatus object to get the status from
 * @return the status recorded in the message
 */
char messagestatus_get_status(MessageStatus * messagestatus) {
	return messagestatus->status;
}

/**
 * Serialize the Status data in JSON format.
 *
 * @param messagestatus The object for serialization
 * @param buffer Memory buffer to store the result in
 */
void messagestatus_serialize(MessageStatus * messagestatus, Buffer * buffer) {
	Json * json;
	Buffer * encrypted;
	Buffer * encoded;
	Buffer * iv;
	Buffer * toEncrypt;
	Buffer * vEncKey;

	json = json_new();

	// Encrypted data
	toEncrypt = buffer_new(1);

	// status
	buffer_append(toEncrypt, & messagestatus->status, 1);

	// extraData
	buffer_append_buffer_lengthprepend(toEncrypt, messagestatus->extraData);

	iv = buffer_new(CRYPTOSUPPORT_IV_SIZE);
	cryptosupport_generate_iv(iv);

	encrypted = buffer_new(0);
	vEncKey = shared_get_verifier_enc_key(messagestatus->shared);
	cryptosupport_encrypt(vEncKey, iv, toEncrypt, encrypted);

	json_add_integer(json, "sessionId", messagestatus->sessionId);

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
 * Set the session ID to return to Pico.
 *
 * @param messagestatus The object to set the session ID for
 * @param The integer to set the session ID to
 */
void messagestatus_set_session_id(MessageStatus * messagestatus, int sessionId) {
	messagestatus->sessionId = sessionId;
}

/**
 * Deserialize a Status JSON string (likely received by the protocol from
 * the Service) and store the data collected from it into the messagestatus
 * object.
 *
 * The function will return false if the deserialization fails. Reasons
 * for failure include:
 *  - A malformed JSON string
 *  - Failure for the encrypted data section to decrypt properly (the data is
 *    encrypted using GCM, which includes a MAC and so incorrect decryption
 *    will be identified)
 *
 * @param messagestatus The MessageStatus object to store the deserialized
 *                        data into
 * @param buffer The JSON string to deserialize
 * @return true if the message was deserialized correctly, false o/w.
 */
bool messagestatus_deserialize(MessageStatus * messagestatus, Buffer const * buffer) {
	Json * json;
	char const * value;
	Buffer * cleartext;
	size_t start;
	size_t next;
	size_t length;
	bool result;
	Buffer * pEncKey;
	Buffer * iv;
	Buffer * encryptedData;

	iv = buffer_new(0);
	encryptedData = buffer_new(0);
	json = json_new();
	result = json_deserialize_buffer(json, buffer);

	if (result) {
		if (json_get_type(json, "sessionId") == JSONTYPE_INTEGER) {
			messagestatus->sessionId = json_get_integer(json, "sessionId");
		}
		else {
			LOG(LOG_ERR, "Missing sessionId\n");
			result = false;
		}
	}

	if (result) {
		value = json_get_string(json, "iv");
		if (value) {
			base64_decode_string(value, iv);
		}
		else {
			LOG(LOG_ERR, "Missing iv\n");
			result = false;
		}
	}

	if (result) {
		value = json_get_string(json, "encryptedData");
		if (value) {
			base64_decode_string(value, encryptedData);
		}
		else {
			LOG(LOG_ERR, "Missing encryptedData\n");
			result = false;
		}
	}

	cleartext = buffer_new(0);
	if (result) {
		pEncKey = shared_get_verifier_enc_key(messagestatus->shared);
		result = cryptosupport_decrypt(pEncKey, iv, encryptedData, cleartext);
	}

	start = 0;
	if (result) {
		length = buffer_get_pos(cleartext);
		next = start + sizeof(char);
		if ((next > start) && (next <= length)) {
			messagestatus->status = buffer_get_buffer(cleartext)[0];
			LOG(LOG_INFO, "MessageStatus returned status is: %d\n", messagestatus->status);
			start = next;
		}
		else {
			LOG(LOG_ERR, "MessageStatus status value missing\n");
			messagestatus->status = MESSAGESTATUS_ERROR;
			result = false;
		}
	}
	if (result) {
		next = buffer_copy_lengthprepend(cleartext, start, messagestatus->extraData);
		if (next > start) {
			start = next;
		}
		else {
			LOG(LOG_ERR, "Error deserializing decrypted length-prepended extraData data\n");
			result = false;
		}
	}

	if (result) {
		// If we got here successfully, it is expected to have consumed the whole buffer
		result = start == buffer_get_pos(cleartext);
	}

	buffer_delete(cleartext);
	buffer_delete(iv);
	buffer_delete(encryptedData);

	json_delete(json);

	return result;
}

/**
 * Return the extra data sent by the service as part of the MessageStatus.
 *
 * @param messagestatus The MessageStatus object to get the data from
 * @return Buffer object containing the extra data. This should not be freed.
 */
Buffer * messagestatus_get_extra_data(MessageStatus * messagestatus) {
	return messagestatus->extraData;
}

/**
 * Set the extra data to be sent by the Service as part of the MessageServiceAuth.
 *
 * @param messagestatus The MessageStatus object to set the data for
 * @param Buffer object containing the extra data. Thd data will be copied, so
 *        it's safe to free this after the call..
 */
void messagestatus_set_extra_data(MessageStatus * messagestatus, Buffer const * extraData) {
	buffer_clear(messagestatus->extraData);
	buffer_append_buffer(messagestatus->extraData, extraData);
}

/** @} addtogroup Message */

