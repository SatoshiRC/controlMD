/*
 * ControlMD.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Satoshi Ohya
 */
#ifndef CONTROL_MD_HPP_
#define CONTROL_MD_HPP_

#include"tim.h"
#include<map>

typedef struct{
  TIM_HandleTypeDef *pTim;
  uint32_t pChannel;
  TIM_HandleTypeDef *nTim;
  uint32_t nChannel;
  uint16_t maxCounterPeriod;
}MD_t;


class ControlMD{
public:
  ControlMD();
  ~ControlMD();
  void addMD(uint8_t number, TIM_HandleTypeDef *pTim, uint32_t pChannel, TIM_HandleTypeDef *nTim, uint32_t nChannel);
  void init(uint8_t number, double maxSpeedPercent);
  void start(uint8_t number);
  void stop(uint8_t number);
  void resetSpeed(uint8_t number);
  uint8_t setSpeed(uint8_t number, int16_t speed);
  void setSpeedPercent(uint8_t number , int8_t percent);

private:
  std::map<uint8_t, MD_t> MD;
};

#endif /*CONTROL_MD_HPP_*/
