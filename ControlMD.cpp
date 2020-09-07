/*
 * ControlMD.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Satoshi Ohya
 */

 #include"ControlMD.hpp"

 ControlMD::ControlMD(){

 }

 ControlMD::~ControlMD(){
	 MD.clear();
 }

void ControlMD::addMD(uint8_t number, TIM_HandleTypeDef *pTim, uint32_t pChannel, TIM_HandleTypeDef *nTim, uint32_t nChannel){
  MD_t tmpArgument = {pTim , pChannel , nTim , nChannel , 0};
  MD.insert(std::make_pair(number, tmpArgument));
}

void ControlMD::init(uint8_t number, double maxSpeedPercent){
  if(MD.find(number) == MD.end()){
	  return;
  }
  if(MD[number].pTim->Instance->PSC >= MD[number].nTim->Instance->PSC){
	  MD[number].maxCounterPeriod = (uint16_t)MD[number].nTim->Init.Period * maxSpeedPercent / 100;
  }else{
	  MD[number].maxCounterPeriod = (uint16_t)MD[number].pTim->Init.Period * maxSpeedPercent / 100;
  }
}

void ControlMD::start(uint8_t number){
	if(MD.find(number) == MD.end()){
		return;
	}
	HAL_TIM_PWM_Start(MD[number].pTim, MD[number].pChannel);
	HAL_TIM_PWM_Start(MD[number].nTim, MD[number].nChannel);
	resetSpeed(number);
}

void ControlMD::stop(uint8_t number){
	if(MD.find(number) == MD.end()){
		return;
	}
	resetSpeed(number);
	HAL_TIM_PWM_Stop(MD[number].pTim, MD[number].pChannel);
	HAL_TIM_PWM_Stop(MD[number].nTim, MD[number].nChannel);
}

void ControlMD::resetSpeed(uint8_t number){
	if(MD.find(number) == MD.end()){
		return;
	}
	__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, 0);
	__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, 0);
}

uint8_t ControlMD::setSpeed(uint8_t number, int16_t speed){
	if(MD.find(number) == MD.end()){
		return 1;
	}
	if(speed < -MD[number].maxCounterPeriod or MD[number].maxCounterPeriod < speed){
    resetSpeed(number);
     return 1;
   }
	if(speed < 0){
		__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, 0);
		__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, -speed);
	} else if(0 < speed){
		__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, speed);
		__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, 0);
	} else {
		__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, 0);
		__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, 0);
	}
  return 0;
}

void ControlMD::setSpeedPercent(uint8_t number , int8_t percent){
	if(MD.find(number) == MD.end()){
		return;
	}
	setSpeed(number, (int16_t)percent * MD[number].maxCounterPeriod * 0.01);
}
