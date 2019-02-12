#include "includes.h"

TEST_CASE("multi_instance", "[multi_instance]]")
{
    prepare_logdir();

    std::string global_file = "logs/global";
    spdlog::create<spdlog::sinks::basic_file_sink_mt>("logger", global_file);

    auto instance_1 = spdlog::new_instance();
    std::string instance_1_file = "logs/instance_1";
    spdlog::create<spdlog::sinks::basic_file_sink_mt>("logger", instance_1_file);

    auto instance_2 = spdlog::new_instance();
    std::string instance_2_file = "logs/instance_2";
    spdlog::create<spdlog::sinks::basic_file_sink_mt>("logger", instance_2_file);

    auto logger_2 = spdlog::get("logger");
    logger_2->set_pattern("%v");
    logger_2->info("instance 2");
    logger_2->flush();

    spdlog::unset_instance();

    auto logger = spdlog::get("logger");
    logger->set_pattern("%v");
    logger->info("global");
    logger->flush();

    spdlog::set_instance(instance_1);

    auto logger_1 = spdlog::get("logger");
    logger_1->set_pattern("%v");
    logger_1->info("instance 1");
    logger_1->flush();

    REQUIRE(file_contents(global_file) == std::string("global\n"));
    REQUIRE(file_contents(instance_1_file) == std::string("instance 1\n"));
    REQUIRE(file_contents(instance_2_file) == std::string("instance 2\n"));
}
