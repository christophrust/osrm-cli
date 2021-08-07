#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"
#include <boost/algorithm/string.hpp>

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>



using namespace osrm;

void route_osrm(double lon1, double lat1, double lon2, double lat2, const OSRM * osrm){

    RouteParameters params;

    //
    params.coordinates.push_back({util::FloatLongitude{lon1}, util::FloatLatitude{lat1}});
    params.coordinates.push_back({util::FloatLongitude{lon2}, util::FloatLatitude{lat2}});

    // Response is in JSON format
    engine::api::ResultT result = json::Object();

    // Execute routing request, this does the heavy lifting
    const auto status = osrm->Route(params, result);

    auto &json_result = result.get<json::Object>();
    if (status == Status::Ok)
        {

            // get first route
            auto &routes = json_result.values["routes"].get<json::Array>();
            auto &route = routes.values.at(0).get<json::Object>();
            const auto distance = route.values["distance"].get<json::Number>().value;
            const auto duration = route.values["duration"].get<json::Number>().value;

            // and distance from input coordinates to waypoints
            auto &waypoints = json_result.values["waypoints"].get<json::Array>();
            auto &waypoint1 = waypoints.values.at(0).get<json::Object>();
            auto &waypoint2 = waypoints.values.at(1).get<json::Object>();
            const auto d1 = waypoint1.values["distance"].get<json::Number>().value;
            const auto d2 = waypoint2.values["distance"].get<json::Number>().value;

            // write out
            std::cout << distance << ";" << duration << ";" << d1 << ";" << d2 << std::endl;


            // std::cout << "Distance: " << distance << " meter\n";
            // std::cout << "Duration: " << duration << " seconds\n";
           // return 0;
        }
    else if (status == Status::Error)
        {
            const auto code = json_result.values["code"].get<json::String>().value;
            const auto message = json_result.values["message"].get<json::String>().value;

            std::cout << "NA;NA;NA;NA" << std::endl;

            //std::cout << "Code: " << code << "\n";
            //std::cout << "Message: " << code << "\n";
            //return 1;
        }

}



// std::vector<std::string> parse_line(const std::string& str, double & latlon){
//     // std::cout << str << std::endl;

//     std::vector<std::string> strs;

//     boost::split(strs, str, boost::is_any_of(";"));

//     // std::cout << strs[0] << std::endl;

//     return strs;
//     // std::cout << std::stod(strs[1])  << std::endl;
// }



int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " data.osrm\n";
        return EXIT_FAILURE;
    }

    // Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
    EngineConfig config;

    config.storage_config = {argv[1]};
    config.use_shared_memory = false;

    // We support two routing speed up techniques:
    // - Contraction Hierarchies (CH): requires extract+contract pre-processing
    // - Multi-Level Dijkstra (MLD): requires extract+partition+customize pre-processing
    //
    config.algorithm = EngineConfig::Algorithm::CH;
    //config.algorithm = EngineConfig::Algorithm::MLD;

    // Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
    const OSRM osrm{config};

    // The following shows how to use the Route service; configure this service

    std::ifstream in(argv[2]);

    std::string str;
    std::vector<std::string> strs;


    double lat_src, lon_src, lat_dst, lon_dst;
    double lat1, lon1, lat2, lon2;
    int ret;

    std::cout << "ID;distance;duration;d1;d2" << std::endl;

    while (std::getline(in, str)) {

        //parse_line(str);

        boost::split(strs, str, boost::is_any_of(";"));

        lon_src = std::stod(strs[1]);
        lat_src = std::stod(strs[2]);
        lon_dst = std::stod(strs[3]);
        lat_dst = std::stod(strs[4]);

        std::cout << strs[0] << ";";

        route_osrm(lon_src, lat_src, lon_dst, lat_dst, &osrm);

    }


    // for (int i=0; i < 10; i++){

    //     lon1 = 11.0154048;
    //     lat1 = 49.6635;
    //     lon2 = 11.2754048;
    //     lat2 = 49.3635;

    //     ret = route_osrm(lon1, lat1, lon2, lat2, &osrm);

    //     if (ret) {
    //         return EXIT_FAILURE;
    //     }

    // }

    return EXIT_SUCCESS;


}
