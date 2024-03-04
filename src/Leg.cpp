#include <Leg.hpp>



Leg::Leg(){
}


void Leg::setLegLengths(float l1, float l2){
    this->L1_ = l1;
    this->L2_ = l2;
}


void Leg::setDesKneePos(float pos){
    Leg::desKneePos_ = pos;
}
