#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "ts_queue.hpp"
#include "item.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "producer.hpp"
#include "consumer_controller.hpp"

#define READER_QUEUE_SIZE 200
#define WORKER_QUEUE_SIZE 200
#define WRITER_QUEUE_SIZE 4000
#define CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE 20
#define CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE 80
#define CONSUMER_CONTROLLER_CHECK_PERIOD 1000000
#define PRODUCER_THREADS_NUM 4

int main(int argc, char** argv) {
	assert(argc == 4);

	int n = atoi(argv[1]);
	std::string input_file_name(argv[2]);
	std::string output_file_name(argv[3]);

	// TODO: implements main function (Done)
    TSQueue<Item*> *input_queue = new TSQueue<Item*>(READER_QUEUE_SIZE);
    TSQueue<Item*> *worker_queue = new TSQueue<Item*>(WORKER_QUEUE_SIZE);
    TSQueue<Item*> *output_queue = new TSQueue<Item*>(WRITER_QUEUE_SIZE);

    Reader *reader = new Reader(n, input_file_name, input_queue);
    Writer *writer = new Writer(n, output_file_name, output_queue);
    Transformer *transformer = new Transformer;

    std::vector<Producer*> producer(PRODUCER_THREADS_NUM);
    for (int i = 0; i < PRODUCER_THREADS_NUM; i++) {
        producer[i] = new Producer(input_queue, worker_queue, transformer);
    }

    auto *consumerController = new ConsumerController(
            worker_queue, output_queue, transformer,
            CONSUMER_CONTROLLER_CHECK_PERIOD,
            std::round(CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE * WORKER_QUEUE_SIZE / 100.0),
            std::round(CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE * WORKER_QUEUE_SIZE / 100.0)
    );
    reader->start();
    writer->start();

    for (int i = 0; i < PRODUCER_THREADS_NUM; i++) {
        producer[i]->start();
    }
    consumerController->start();

    reader->join();
    writer->join();

    // delete
    delete writer;
    delete reader;
    delete transformer;
    delete consumerController;
    delete input_queue;
    delete worker_queue;
    delete output_queue;

	return 0;
}
