/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/**
 * @author radu.pop, beniamin.tecar, catalin.pop
 */
#ifndef SECURITYSYMJOINREQUEST_H_
#define SECURITYSYMJOINREQUEST_H_

#include "Misc/Convert/Convert.h"
#include "Common/smTypes.h"
#include "Common/NEAddress.h"
#include <iomanip>

using namespace NE::Misc::Convert;
using namespace NE::Misc::Marshall;

namespace Isa100 {
namespace Security {

struct SecuritySymJoinRequest {

		NE::Common::Address64 EUI64JoiningDevice;	// 0

        /**
         * 128_Bit_Challenge_from_new_device is a fresh unique challenge generated by the new device to
         * verify that the security manager is alive.
         */
        Isa100::Common::Uint128 bitChallengeFromNewDevice; // 1

        /**
         * The Key_info_from_new_device shall be as defined in I.4.1
         */
        Uint8 keyInfoFromNewDevice; // 2

        /**
         * The algorithm identifier shall be composed of 4 bits for the public key
         * algorithm and options, and 4 bits for the symmetric key algorithm and mode.
         * The value of 0x0 shall be reserved. A symmetric key algorithm of 0x01 corresponding
         * to AES_CCM* shall be the only symmetric algorithm and mode supported for the join process.
         */
        Uint8 algorithmIdentifier; // 3

        /**
         * The MIC is of 32 bits in length and is computed over the elements 1 through 4,
         * using the join key and the 13 most significant octets of the challenge as nonce.
         */
        Uint32 mic; // 4

        void marshall(OutputStream& stream) {
        	EUI64JoiningDevice.marshall(stream);
            for (Uint8 i = 0; i < Isa100::Common::CHALLENGE_BYTES_NO; i++) {
                stream.write(bitChallengeFromNewDevice[i]);
            }
            stream.write(keyInfoFromNewDevice);
            stream.write(algorithmIdentifier);
            stream.write(mic);
        }

        void unmarshall(InputStream& stream) {
        	EUI64JoiningDevice.unmarshall(stream);
            for (Uint8 i = 0; i < Isa100::Common::CHALLENGE_BYTES_NO; i++) {
                stream.read(bitChallengeFromNewDevice[i]);
            }
            stream.read(keyInfoFromNewDevice);
            stream.read(algorithmIdentifier);
            stream.read(mic);
        }

        void toString( std::string &joinRequestString) {
            std::ostringstream stream;
            stream << "SecuritySymJoinRequest {";

            for (Uint8 i = 0; i < Isa100::Common::CHALLENGE_BYTES_NO; i++) {
                stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int) bitChallengeFromNewDevice[i];
                if (i % 2 == 1 && i != Isa100::Common::CHALLENGE_BYTES_NO - 1) {
                    stream << ':';
                }
            }

            stream << "BitChallengeFromNewDevice=" << bitChallengeFromNewDevice;
            stream << ", keyInfoFromNewDevice=" << keyInfoFromNewDevice;
            stream << ", algorithmIdentifier=" << algorithmIdentifier;
            stream << ", mic=" << mic;
            stream << "}";
            joinRequestString = stream.str();
        }
};

}
}

#endif /*SECURITYSYMJOINREQUEST_H_*/