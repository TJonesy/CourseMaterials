#include "Task.hpp"
#include <random>

namespace
{
  int getInt(int low, int high)
  {
    thread_local std::random_device rd;
    thread_local std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(low, high);
    return dist(mt);
  }

float getFloat(float mean, float dev)
  {
    thread_local std::random_device rd;
    thread_local std::mt19937 mt(rd());
    std::normal_distribution<float> dist(mean, dev);
    return dist(mt);
  }

  cs3100::Task::Type getType()
  {
    auto t = getInt(0,1);
    return t == 0 ? cs3100::Task::Type::CPU : cs3100::Task::Type::IO;
  }

  cs3100::Task getTask( cs3100::Task::Type boundBy,int maxDevice,int maxPage)
  {
    auto type = getType();
    auto duration = type==boundBy?getFloat(20.0f,5.0f):getFloat(5.0f,1.0f);
    auto max = type==cs3100::Task::Type::CPU?maxPage:maxDevice;
    return cs3100::Task(duration, getInt(0, max), type);
  }

}



namespace cs3100
{
  Job::Job(float t,int devices,int pages):creationTime(t),cur(),tasks()
  {
    //jobs might be cpu or io bound, we choose evenly between the two options
    auto bindType = getType();
    auto numTasks = getInt(10,100);
    for(int i=0;i<numTasks;++i)
    {
       tasks.push_back(getTask(bindType,devices,pages));
    }
    cur = tasks.begin();
  }
}
