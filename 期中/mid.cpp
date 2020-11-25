#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
/**
 *  輸出錯誤訊息。
 *
 *  @param const std::string& cmd   程式執行時的名稱
 *  @return none.
 *  @since  0.1.0
 **/
void error(const std::string& cmd, const std::string& msg) {
    std::cout << "Error: " << msg << std::endl;
    std::cout << " type '" << cmd << " -?' to see usage." << std::endl;

    exit(-1);
} // error()

/**
 *  印出十進制轉換提示字串。建構並輸出一個格式為:
 *    Decimal number #dec in #base: #num
 *  的提示字串；其中:
 *      #dec  代表十進制數字字串；
 *      #base 是轉換基底 (base)，如二進，八進制等；
 *      #num  是轉換後數字字串
 *  @param const std::string& dec   十進制數字字串
 *  @param const std::string& base  數字基底字串
 *  @param const std::string& num   轉換後數字字串
 *  @return None.
 *  @since  0.1.0
 **/
void numInBase(
    const std::string& dec,
    const std::string& base,
    const std::string& num
) {
    std::cout
        << "Decimal number "
        << dec
        << base
        << num
        << std::endl;
} // numInBase

/**
 *  將十進制數字字串轉換為二進制數字字串。
 *
 *  @param const std::string& dec   十進制數字字串
 *  @return std::string             二進制數字字串
 *  @since  0.1.0
 **/
std::string dec2bin(const std::string& dec) {
    std::stringstream bin;

    bin << std::bitset<16>(std::stoi(dec));

    numInBase(dec, " in 16 bits binary: ", bin.str());

    return bin.str();
} // decToBin()

/**
 *  將十進制數字字串轉換為十六進制數字字串。
 *
 *  @param const std::string& dec   十進制數字字串
 *  @return std::string             十六進制數字字串
 *  @since  0.1.0
 **/
std::string dec2hex(const std::string& dec) {
    std::stringstream hex;

    hex << "0x" << std::hex << std::stoi(dec);

    numInBase(dec, " in hexadecimal: ", hex.str());

    return hex.str();
} // decToHex()

/**
 *  將十進制數字字串轉換為八進制數字字串。
 *
 *  @param const std::string& dec   十進制數字字串
 *  @return std::string             八進制數字字串
 *  @since  0.1.0
 **/
std::string dec2oct(const std::string& dec) {
    std::stringstream oct;

    oct << "0" << std::oct << std::stoi(dec);

    numInBase(dec, " in octal: ", oct.str());

    return oct.str();
} // decToOct()

/**
 *  印出轉換十進制提示字串。建構並輸出一個格式為:
 *    #base number #num in decimal: #dec
 *  的提示字串；其中:
 *      #base 是數字基底 (base)，如二進，八進制等；
 *      #num  是數字字串
 *      #dec  代表十進制數字字串；
 *  @param const std::string& base  數字基底字串
 *  @param const std::string& num   數字字串
 *  @param const std::string& dec   十進制數字字串
 *  @return None.
 *  @since  0.1.0
 **/
void numInDec(
    const std::string& base,
    const std::string& num,
    const std::string& dec
) {
    std::cout
        << base
        << " number "
        << num
        << " in decimal: "
        << dec
        << std::endl;
} // numInDec()

/**
 *  將二進制數字字串轉換為十進制數字字串。
 *
 *  @param const std::string& bin   二進制數字字串
 *  @return std::string             十進制數字字串
 *  @since  0.1.0
 **/
std::string bin2dec(const std::string& bin) {
    std::stringstream dec;

    dec << std::stol(bin, nullptr, 2);

    numInDec("Binary", bin, dec.str());

    return dec.str();
} // bin2dec()

/**
 *  將八進制數字字串轉換為十進制數字字串。
 *
 *  @param const std::string& oct   八進制數字字串
 *  @return std::string             十進制數字字串
 *  @since  0.1.0
 **/
std::string oct2dec(const std::string& oct) {
    std::stringstream dec;

    dec << std::stol(oct, nullptr, 8);

    numInDec("Octal", oct, dec.str());

    return dec.str();
} // oct2dec()

/**
 *  將十六進制數字字串轉換為十進制數字字串。
 *
 *  @param const std::string& hex   十六進制數字字串
 *  @return std::string             十進制數字字串
 *  @since  0.1.0
 **/
std::string hex2dec(const std::string& hex) {
    std::stringstream dec;

    dec << std::stol(hex, nullptr, 16);

    numInDec("Hexadecimal", hex, dec.str());

    return dec.str();
} // hex2dec()

/**
 *  檢查 *str* 是不是數字字串。利用 C++ 的 stol (string-to-long)
 *  函數檢查輸入的字串是不是合理的 *數字字串*。
 *
 *  @param const std::string& str   要檢查的字串
 *  @return bool    true    合法的數字字串
 *                  false   含有非數字字元的字串
 *  @since  0.1.0
 **/
bool is_number(const std::string& str, int base = 10) {
    size_t sz;

    try {
        std::stol(str, &sz, base);
    }
    catch (const std::invalid_argument& e) {
        ;
    }

    return (sz == str.length());
} // is_number()

/**
 *  十進制-二進制數字互相轉換測試函數。
 *
 *  @param const std::string& dec   十進制數字字串
 *  @return none.
 *  @since  0.1.0
 **/
bool test_dec_bin(const std::string& cmd, const std::string& dec) {
    std::string bin;
    std::string num;

    std::cout << "C:\\" << cmd << " -b " << dec << std::endl;
    bin = dec2bin(dec);

    std::cout << "C:\\" << cmd << " -B " << num << std::endl;
    num = bin2dec(bin);

    std::cout << std::endl;

    return (dec == num);
} // test_dec_bin();

/**
 *  十進制-十六進制數字互相轉換測試函數。
 *
 *  @param const std::string& dec   十進制數字字串
 *  @return none.
 *  @since  0.1.0
 **/
bool test_dec_hex(const std::string& cmd, const std::string& dec) {
    std::string hex;
    std::string num;

    std::cout << "C:\\" << cmd << " -h " << dec << std::endl;
    hex = dec2hex(dec);

    std::cout << "C:\\" << cmd << " -H " << num << std::endl;
    num = hex2dec(hex);

    std::cout << std::endl;

    return (dec == num);
} // test_dec_hex();

/**
 *  十進制-八進制數字互相轉換測試函數。
 *
 *  @param const std::string& dec   十進制數字字串
 *  @return none.
 *  @since  0.1.0
 **/
bool test_dec_oct(const std::string& cmd, const std::string& dec) {
    std::string oct;
    std::string num;

    std::cout << "C:\\" << cmd << " -o " << dec << std::endl;
    oct = dec2oct(dec);

    std::cout << "C:\\" << cmd << " -O " << num << std::endl;
    num = oct2dec(oct);

    std::cout << std::endl;

    return (dec == num);
} // test_dec_oct();

/**
 *  輸出程式使用範例。
 *
 *  @param const std::string& cmd   程式執行時的名稱
 *  @param const std::string& dec   十進制數字字串
 *  @return none.
 *  @since  0.1.0
 **/
void sample_usage(const std::string& cmd, const std::string& dec) {
    std::cout << "Examples: " << std::endl << std::endl;

    test_dec_bin(cmd, dec) ? (void)0 : error(cmd, "Dec-Bin converting error!");
    test_dec_hex(cmd, dec) ? (void)0 : error(cmd, "Dec-Hex converting error!");
    test_dec_oct(cmd, dec) ? (void)0 : error(cmd, "Dec-Oct converting error!");
} // sample_usage()

/**
 *  輸出程式使用說明。
 *
 *  @param const std::string& cmd   程式執行時的名稱
 *  @return none.
 *  @since  0.1.0
 **/
void usage(const std::string& cmd) {
    std::string num;

    std::cout << cmd << " v0.1.0 " << std::endl;
    std::cout << "usage: " << std::endl;
    std::cout << " -H hexadecimal to decimal." << std::endl;
    std::cout << " -h decimal to hexadecimal." << std::endl;
    std::cout << " -O octal to decimal." << std::endl;
    std::cout << " -o decimal to octal." << std::endl;
    std::cout << " -B binary to decimail." << std::endl;
    std::cout << " -b decimal to binary." << std::endl;
    std::cout << " -? print this usage." << std::endl;
    std::cout << " -x some usage samples." << std::endl;
    std::cout << std::endl;

    sample_usage(cmd, "101");
} // usage()

/**
 *  *傾印* 命令列參數。將命令列參數全數傾印在螢幕上。
 *
 *  @param const int argc           命令列參數個數
 *  @param const char *argv[]       命令列參數陣列
 *  @return int
 *  @since  0.1.0
 **/
void args_dump(int argc, char *argv[]) {
    // 列印所有的 *命令列參數*
    std::cout << "command line arguments: argv[i]" << std::endl;

    for (int i = 0; i < argc; i ++) {
        std::cout
            << std::setfill(' ') << std::setw(3)
            << i << ": "
            << argv[i] << std::endl;
    }
}

/**
 *  程式進入點。
 *
 *  @param const int argc           命令列參數個數
 *  @param const char *argv[]       命令列參數陣列
 *  @return int
 *  @since  0.1.0
 **/
int main(int argc, char *argv[]) {
    // args_dump(argc, argv); // 如果想要檢查程式接收到的
                              // 命令列參數；可以將前面的
                              // 註解拿掉

 std::string arg = argv[1];
 
 if( argc !=3 && argc !=2)
 {
 	error( argv[0],argv[1]);
 	usage(argv[0]);
 }
 
if(argc==2)
{
	if(arg=="-?")
	{
		usage(argv[0]);
	}
	else
	{
	error( argv[0],argv[1]);
	usage(argv[0]);
	}
}

if(argc==3)
{
	if(arg=="-b")
	{
		dec2bin(argv[2]);
	}
	else if(arg=="-B")
	{
		bin2dec(argv[2]); 
	}
	else if(arg=="-h")
	{
		dec2hex(argv[2]);
	}
	else if(arg=="-H")
	{
		hex2dec(argv[2]);
	}
	else if(arg=="-o")
	{
		dec2oct(argv[2]);
	}
	else if(arg=="-O")
	{
		oct2dec(argv[2]);
	}
	else if(arg=="-x")
	{
		sample_usage(argv[0],argv[2]);
	}
	else
	{
		error( argv[0],argv[1]);
	usage(argv[0]);
	}
	
}

return 0;
}
    // 請在這裡放上你的程式碼
    // 使得編譯後的執行檔行為和 sample.exe 一致。
    //
    // 簡單說 *程式 (sample.exe) 行為* 遵循下列規則：
    //
    //  1. sample -? : 呼叫 usage()
    //
    //  1. sample -b dec : 呼叫 dec2bin(dec)
    //
    //  1. sample -B bin : 呼叫 bin2dec(bin)
    //
    //  1. sample -h dec : 呼叫 dec2hex(dec)
    //
    //  1. sample -H hex : 呼叫 hex2dec(hex)
    //
    //  1. sample -o dec : 呼叫 dec2oct(dec)
    //
    //  1. sample -O oct : 呼叫 oct2dec(oct)
    //
    //  1. sample -x dec : 呼叫 sample_usage(dec)
    //
    //  執行範例
    //
    //  1. 可以在命令列執行 test.bat
    //
    //     C:\path_to\UI_Practice\9th\midterm> test.bat
    //
    //     看 sample.exe 的執行範例
    //
    //  1. 或，在命令列執行：
    //
    //     C:\path_to\UI_Practice\9th\midterm> sample -?
    //
    //     來看 sample.exe 的用法；再自行嘗試。
    //
    //  測試程式
    //
    //  1. 假設你的程式編譯成 mywork.exe，在命令列執行：
    //
    //     C:\path_to\UI_Practice\9th\midterm> test.bat mywork
    //
    //     看 mywork.exe 的測試執行結果
    //
    //  上傳程式 --- 對自己的程式滿意後：
    //
    //  1. 同步到自己的 GitHub 專案裡
    //
    //  1. 為確保一致的截止時限，請同時將這份檔案 (midterm.cpp)
    //     上傳一份到數位學習平台。
    //
    //  作業提示
    //
    //  1. GitHub 的版本，你可以持續更新
    //
    //  1. 截止期限前，你可以多次上傳檔案到數位學習平台
    //
    //  祝
    //    一切順利
} // main()
