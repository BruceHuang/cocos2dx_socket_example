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

#ifndef _ProtoMessageDelegate_h_
#define _ProtoMessageDelegate_h_

#include "MessageRequest.pb.h"
#include "MessageResponse.pb.h"
#include "IMessageSerializer.h"
#include "BinarySerializer.h"

class ProtoMessageDelegate {
public:
    // simple method to enqueue proto messages
    void enqueuProtoMessage() {
        auto message = _protoMessageQueue.front();
        _protoMessageQueue.pop();
        processProtoMessage(message);
    }
    
    // cocos2dx OpenGL is not thread safe, so we need to synchronize with cocos2dx thead
    // put messages to queue and then enqueue with Scheduler
    void onProtoMessageReceive(MessageResponse &message) {
        _protoMessageQueue.push(message);
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(ProtoMessageDelegate::enqueuProtoMessage, this));
    }
    
    // this method must be implemented in Every scene that uses communication with sockets
    virtual void processProtoMessage(MessageResponse &message) = 0;
    
    // this method just wrap our message to MessageRequest to send it next to server
    const std::string wrapMessage(eCommunicationMessageType msgType, google::protobuf::MessageLite &msgBody) {
        MessageRequest msg;
        msg.set_messagetype(msgType);
        msg.set_messagebody(msgBody.SerializeAsString());
        return _serializer->serialize(msg);
    }
private:
    IMessageSerializer *_serializer = new BinarySerializer();
    std::queue<MessageResponse> _protoMessageQueue;
};

#endif
