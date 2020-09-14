


#define DOCTEST_CONFIG_IMPLEMENT

#include <rtc/core/ray.hpp>
#include <rtc/core/collision_data.hpp>
#include <rtc/core/sphere.hpp>
#include <doctest/doctest.h>

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// Writes a generic GLM vector type to a stream.
namespace glm {
    template<int D, typename T, glm::qualifier P>
    std::ostream &operator<<(std::ostream &os, const glm::vec<D, T, P> &v) {
        return os << glm::to_string(v);
    }
}
int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

    // defaults
//    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("order-by", "name");            // sort the test cases by their name
    context.setOption("no_colors", false);
    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}

TEST_SUITE("ray")
{
    SCENARIO("Creating and querying a ray")
    {
        GIVEN("origin=(1,2,3)")
        {
            glm::vec3 origin(1,2,3);
            GIVEN("direction=(1,0,-1)")
            {
                glm::vec3 direction(1,0,-1);
                glm::to_string(direction);

                WHEN("r=Ray(origin.direction)")
                {
                    ray r(origin,direction);
                    THEN("r.origin==origin")
                    {
                        REQUIRE_EQ(r.pos,origin);
                    }
                    AND_THEN("r.direction==direction after normalization")
                    {
                        REQUIRE_EQ(r.dir,glm::normalize(direction));
                    }
//                    std::cout<<glm::to_string(direction)<<std::endl;
                }
            }
        }
    }

    SCENARIO("Computing points from a distance")
    {
        GIVEN("r=Ray(2,3,4),(1,0,0))")
        {
            ray r(glm::vec3(2,3,4),glm::vec3(1,0,0));
            THEN("position at t=0 is (2,3,4)")
            {
                REQUIRE_EQ(r(0),glm::vec3(2.0f,3.0f,4.0f));
            }
                    THEN("position at t=1 is (3,3,4)")
            {
                        REQUIRE_EQ(r(1),glm::vec3(3.0f,3.0f,4.0f));
            }
                    THEN("position at t=-1 is (1,3,4)")
            {
                        REQUIRE_EQ(r(-1),glm::vec3(1.0f,3.0f,4.0f));
            }
                    THEN("position at t=2.5 is (3,3,4)")
            {
                        REQUIRE_EQ(r(2.5),glm::vec3(4.5,3.0f,4.0f));
            }
        }
    }

    SCENARIO("A ray intersect with sphere at 2 points")
    {
        GIVEN("r=Ray(0,0,-5),(0,0,1))")
        {
            GIVEN("s=sphere()")
            {
                ray r(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1));
                sphere s;
                collision_data d=s.collide(r);
                WHEN("d=s.intersect(r)")
                {
                    THEN("number of intersections is 2")
                    {
                        REQUIRE_EQ(d.t.size(),2);
                    }
                    AND_THEN("first intersction is at (0,0,-1)")
                    {
                        REQUIRE_EQ(r(d.t[0]),glm::vec3(0,0,-1));
                    }
                    AND_THEN("second intersction is at (0,0,1)")
                    {
                        REQUIRE_EQ(r(d.t[1]),glm::vec3(0,0,1));
                    }
                }
            }
        }
    }


    SCENARIO("A ray miss a sphere")
    {
                GIVEN("r=Ray(0,10,-5),(0,0,1))")
        {
                    GIVEN("s=sphere()")
            {
                ray r(glm::vec3(0, 10, -5), glm::vec3(0, 0, 1));
                sphere s;
                collision_data d=s.collide(r);
                        WHEN("d=s.intersect(r)")
                {
                            THEN("number of intersections is 0")
                    {
                                REQUIRE_EQ(d.t.size(),0);
                    }
                }
            }
        }
    }
}