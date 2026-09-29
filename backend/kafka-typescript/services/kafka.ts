import { API, createKafkaClient } from "kafka-ts";
import type { ProducerOptions } from "kafka-ts/dist/producer/producer";

const kafka = createKafkaClient({
    bootstrapServers:[{host:"localhost",port:9092}],
    clientId:"kafka-experiment"
})

export async function ConsumeMessage(topicName:string,consumerGroup?:string){
    kafka.startConsumer({topics:[topicName],groupId:consumerGroup,onBatch:(messages)=>
    {
       messages.forEach(message => {
        console.log(`Key:${message.key}`)
        console.log(`value:${message.value}`)
        console.log(`partition:${message.partition}`)
        console.log(`offset:${message.offset}`)
       });
    }
    })
}

export async function CreateTopic(topicName:string,numPartitions:number)
{
    const cluster = kafka.createCluster();
    await cluster.connect();

    const { controllerId } = await cluster.sendRequest(API.METADATA, {
        allowTopicAutoCreation: false,
        includeTopicAuthorizedOperations: false,
        topics: [],
    });
    console.log(controllerId);
    await cluster.sendRequestToNode(controllerId)(API.CREATE_TOPICS, {
        validateOnly: false,
        timeoutMs: 10_000,
        topics: [
            {
                name: topicName,
                numPartitions: numPartitions,
                replicationFactor: 1,
                assignments: [],
                configs: [],
            },
        ],
    });

    await cluster.disconnect();
}


export async function ProduceMessage(message:{key:string,value:string},topicName:string)
{
    const producer = kafka.createProducer({})
    await producer.send([{topic:topicName,key:message.key,value:message.value}])
}