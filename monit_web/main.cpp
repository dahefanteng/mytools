#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <iostream>
#include <curlpp/Infos.hpp>
#include <stdio.h>


int probe(){
	long code;
  	try
  	{
		curlpp::options::Url myUrl(std::string("https://oss-cn-north-1.unicloudsrv.com"));
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

int main(){
	probe();
}
