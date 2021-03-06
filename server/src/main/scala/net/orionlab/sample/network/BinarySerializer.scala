// The MIT License (MIT)
//
// Copyright (c) 2015 Alexander Shniperson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

package net.orionlab.sample.network

import ProtoMessages.{CommunicationMessageTypeBase, MessageResponseBase}
import com.google.protobuf.{ByteString, CodedOutputStream}
import ProtoMessages.MessageRequestBase.MessageRequest
import java.util.logging.Level

object BinarySerializer extends IMessageSerializer {

  override def Serialize(message: MessageResponseBase.MessageResponse): Array[Byte] = {
    val result = new Array[Byte](message.getSerializedSize() + 4)
    try {
      val outStream = CodedOutputStream.newInstance(result)
      outStream.writeFixed32NoTag(message.getSerializedSize())
      message.writeTo(outStream)
    } catch {
      case ex: Exception => log.log(Level.SEVERE, "", ex)
    }
    result
  }

  override def Deserialize(serializedMessage: Array[Byte]): Option[MessageRequest] = {
    try {
      if (serializedMessage == null || serializedMessage.length == 0) {
        val builder = MessageRequest.newBuilder()
        builder.setMessageType(CommunicationMessageTypeBase.eCommunicationMessageType.cmtNone)
        builder.setMessageBody(ByteString.EMPTY)
        Some(builder.build())
      } else {
        Some(MessageRequest.parseFrom(serializedMessage))
      }
    } catch {
      case ex: Throwable =>
        log.log(Level.SEVERE, "", ex)
        None
    }
  }
}
