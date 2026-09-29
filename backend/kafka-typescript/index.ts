import { ProduceMessage,ConsumeMessage,CreateTopic } from "./services/kafka"

interface Message {
    topicName:string,
    message:{
        key:string,
        value:string
    }
}
interface SubscribePayload{
    topicName:string,
    consumerGroupName:string
}

interface CreateTopicPayload{
    topicName:string,
    numPartition:number
}
console.log("Starting server...")
const server = Bun.serve({
  port: 3000,
  routes: {
    "/": () => new Response('Bun!'),
    "/message":async (request)=> {
        if(request.method.toLowerCase()==="post"){
            const value = await request.json() as Message;
            await ProduceMessage(value.message,value.topicName);
        }
        return new Response("Topic")
    },
    "/topic":async (request)=>{
     const value = await request.json() as CreateTopicPayload
     console.log(value.topicName,value.numPartition);
     await CreateTopic(value.topicName,value.numPartition);
     return new Response("Success");
    },
    "/subscribe":async (request)=>{
        const value  = await request.json() as SubscribePayload
        await ConsumeMessage(value.topicName,value.consumerGroupName);
        return new Response("Success");
    }

  }
});
