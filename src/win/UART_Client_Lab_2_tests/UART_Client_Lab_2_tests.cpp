//#include "pch.h"
#include "CppUnitTest.h"

//#include "../UART_Client_Lab_2/xml_utils.h"
#include "xml_utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UARTClientLab2tests
{
	TEST_CLASS(UARTClientLab2tests)
	{
	public:
		
		TEST_METHOD(TestCreateXML)
		{
			// Очікуване XML-повідомлення
			std::string expected = "<message>hello</message>";
			std::string result = createXML("hello");

			// Перевірка, що результат відповідає очікуванню
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(TestParseXML)
		{
			// Тест на правильне парсинг XML
			std::string xml = "<message>hello</message>";
			std::string expected = "hello";
			std::string result = parseXML(xml);

			// Перевірка, що результат відповідає очікуванню
			Assert::AreEqual(expected, result);
		}
	};
}
