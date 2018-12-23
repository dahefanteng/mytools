#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <iostream>
#include <curlpp/Infos.hpp>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const std::string URL = "s3.test.com:8080";
const int MON_POINT_NUM = 10;
const int MON_INTERVAL = 6;     //use second for unit.
const int EXPECTED_CODE = 403;
const float THRESHOLD = 0.8;
int status = 1;
enum alertType {broken, recover};

class cycleQueue
{
	private:
		unsigned int size;
		unsigned int current;
		unsigned int healths;
		int* data;
	public:
		cycleQueue(unsigned size):size(size){
			data = new int[size];
			for (int i = 0;i<size;i++){
				data[i] = 1;
			}
			current = 0;
			healths = size;
		}

		~cycleQueue(){
			delete []data;
		}

		void walk(void){
			if (current == size-1){
				current=0;
			} else {
				current++;
			}
		}

		void setValue(int value){
			healths = healths - data[current] +value;
			data[current] = value;
		}
	
		float percent(){
			std::cout << "healthsnum:" << healths << "\n size num:" << size << std::endl;
			return healths/float(size);
		}
};


int probe(){
	long code;
  	try
  	{
		curlpp::options::Url myUrl(URL);
		curlpp::Easy myRequest;
		myRequest.setOpt(myUrl);
		myRequest.perform();
		std::cout << std::endl;
		code = curlpp::infos::ResponseCode::get(myRequest);
		std::cout << code << std::endl;
  	}

        catch( curlpp::RuntimeError &e )
        {
                std::cerr << e.what() << std::endl;
		return -1;
        }

        catch( curlpp::LogicError &e )
        {
                std::cerr << e.what() << std::endl;
		return -1;
        }
	return code;
}

void alert(alertType alert){
	switch(alert) 
	{
		case broken:
			std::cerr << "Your service is not healthy!" << std::endl;
			break;
		case recover:
			std::cerr << "Your service become health again!" << std::endl;
			break;
	}
}

int main(){
	cycleQueue myCycle(MON_POINT_NUM);
	while(1){
		int rt;
		rt = probe();
		if (rt == EXPECTED_CODE){
			myCycle.setValue(1);
		} else {
			myCycle.setValue(0);	
		}
		std::cout << myCycle.percent() << std::endl;
		std::cout << "threshold is " <<THRESHOLD << std::endl;
		if (myCycle.percent() < THRESHOLD && status == 1){
			alert(broken);
			status = 0;
		} else if(myCycle.percent() >= THRESHOLD && status == 0){
			alert(recover);
			status = 1;
		}
		myCycle.walk();
		sleep(MON_INTERVAL);
	}
}
