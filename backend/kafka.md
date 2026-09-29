Topic: Using a topic we can organize data streams.
Logs stream from multiple services (producers) -> logs <--- Consumers
Analytics stream from multiple services (producers) -> analytics <--- Consumers

Topic: Organizing different data streams with a topic
Partition: A topic can be broken into multiple partitions. data can be read from multiple partitions parallely
offset: a message is appended into a particular partition in a topic. Offset is used to identify a message in a parition. Offset is always increasing, even if a message is deleted in a partition, that offset is not reused.

- If an ordered processing of message stream is required, then a key can be attached with the message, so that messages with the same key always go to same partition.
- Kafka streams :Kafka Streams is an application that continuously consumes records from Kafka topics, processes them, optionally maintains state, and usually writes the resulting records to another Kafka topic

- Kafka connect : Kafka connect allows data trasfer between kafka and external systems using reusable connectors (resuable connector part I am not sure of which I shall clarify)

TODO

- How a consumer can consume data if it is connected to cluster after the producer has produced the data.
- Kafka streams and Kafka connect.
- Why kafka gives a warning about containing topics \_ and collision about .
- If I have multiple brokers then how can I list the topics
- What is replication factors (Is the topic is replicated to multiple brokers)
- If a key is used then can parition can be specified in the message. (Partition number can be specified)
- Can an offset be specified (No)

- What is kafka topics
- Topics is broken into partitions
- paritions can be assigned to multiple brokers
- Messages within each partition is ordered
- Serializers : serialize different data types to byte streams.
- A producer message contains :
  - key optional
  - value
  - headers metadata
  - timestamp
- Used kafka-topics and kafka-producer-console to send messages to kafka cluster.

What is bootstrap server in kafka and how client connect to the kafka broker

- Kafka client can connect to any broker, each broker keeps metadata of other brokers, it returns the metadata to the client which other broker contains the partition data.
- Kafka client then connects to the actual broker to get the data

Exercise 1:
Producer and consumer (producer.ts,consumer.ts)
Excercise 2 and Exercise 3:
Create a topic with multiple partitions (created and observed that same key goes to same partition
without key message is assigned in round robin fashion
)
Exercise 4 and Exercise 5:
A partition in a topic is assigned to at most one consumer in a consumer group. A single partition can be assinged to multiple consumers accross consumer groups.
for example:
topic: topic4
partition:0
consumer group : groupA and groupB each having 2 consumers
groupA_consumer1 can be assigned to topic4_partition0
groupA_consumer2 can be assigned to topic4_partition1 (topic4_partition0 can not be assigned to groupA_consumer2 because it is aleardy assigned to consumer1)
groupB_consumer1 can be assigned to topic4_partition0
groupB_consumer2 can be assigned to topic4_partition1

Broker, topic, partition, record, offset
Leader/follower replicas
Controller / KRaft at a conceptual level
Why Kafka is an append-only distributed log rather than simply a “message queue”
Retention by time/size
Log compaction and when you would use it
Producer acks
acks=0, 1, all
min.insync.replicas
Producer retries
Idempotent producer
Batching
linger.ms
compression
partition selection
message keys
ordering guarantee: only within a partition
consumer groups
one partition can be consumed by only one consumer within a group at a time
why adding consumers beyond partition count doesn't improve parallelism
offset commit
auto commit vs manual commit
commit before processing vs after processing
duplicate-processing failure scenarios
consumer rebalancing
what happens when a consumer dies
partition reassignment
heartbeat/session timeout concept
consumer lag
delivery semantics
at-most-once
at-least-once
exactly-once semantics
why exactly-once doesn't magically make external DB operations exactly once
idempotent consumers
deduplication using event IDs
transactional outbox pattern
dual-write problem: DB + Kafka
inbox/deduplication table pattern
retries with retry topics
exponential backoff
dead-letter topics
poison messages
retry ordering problems
partition-key design
hot partitions
uneven key distribution
choosing partition count
partition count versus throughput
replication factor
ISR
leader election
what happens if a broker dies
durability implications of acks=all
schema evolution
JSON vs Avro/Protobuf
backward compatibility
forward compatibility
Schema Registry concept
monitoring producer failures
consumer lag
broker health
throughput
partition skew
rebalance frequency
disk usage

## Skip this sprint

Kafka Streams
Kafka Connect
Schema Registry deep dive
Kafka cluster tuning
Kubernetes
Kafka operator internals
KRaft internals
lock-free algorithms
advanced CPU cache-coherence details

## Kafka topics given by chatgpt

1. Kafka mental model and architecture
2. Producers and delivery semantics
3. Consumers, consumer groups, and rebalancing
4. Partitions, ordering, and keys
5. Offsets and failure recovery
6. At-least-once / at-most-once / exactly-once
7. Idempotency and duplicate handling
8. Retry, dead-letter handling, poison messages
9. Scaling and partition-count decisions
10. Kafka durability and replication
11. Schema evolution
12. Observability and production failure scenarios
