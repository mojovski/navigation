#ifndef EXECUTION_TIMER_H
#define EXECUTION_TIMER_H
#include <ros/console.h>
#include <chrono>
#include <type_traits>


/** Helper to measure execution time
Usage:
{
  ExecutionTimer<std::chrono::milliseconds> timer("map update");
  someFunction(..);

}
**/
template<class Resolution = std::chrono::milliseconds>
class ExecutionTimer {
public:
  /*
    using Clock = std::conditional<std::chrono::high_resolution_clock::is_steady,
                                     std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;
                                     */
    typedef std::chrono::high_resolution_clock Clock;
private:
    const Clock::time_point mStart = Clock::now();

public:
    ExecutionTimer():track_until_descruction(true){name="";};
    ExecutionTimer(std::string name_, bool track_until_descruction_=true){
      name=name_;
      track_until_descruction=track_until_descruction_;
    }

    std::string name;
    bool track_until_descruction;

    ~ExecutionTimer() {
      if (!track_until_descruction)
          return;
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << name << " before destructor took: "
                  << std::chrono::duration_cast<Resolution>( end - mStart ).count()
                  << std::endl;
        //std::cout << strStream.str() << std::endl;
        ROS_DEBUG(strStream.str().c_str());
    }    

    inline void stop() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << name << "took: "
                  << std::chrono::duration_cast<Resolution>(end - mStart).count()
                  << std::endl;
        //std::cout << strStream.str() << std::endl;
        ROS_DEBUG(strStream.str().c_str());
        track_until_descruction=false; //dont print on destructor
    }

}; // ExecutionTimer

#endif // EXECUTION_TIMER_H