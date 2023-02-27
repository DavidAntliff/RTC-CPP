#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <ray_tracer_challenge/tuples.h>

int main() {
    BOOST_LOG_TRIVIAL(info) << "The Ray Tracer Challenge";
    return 0;
}
