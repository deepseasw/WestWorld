#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Miner_Bob,

  ent_Elsa
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Miner_Bob:

    return "����";

  case ent_Elsa:
    
    return "�Ƴ�"; 

  default:

    return "UNKNOWN!";
  }
}

#endif