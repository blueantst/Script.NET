Perl解释器编译说明：
1、Perl的lib库默认路径是和bin目录平级的lib目录，为了支持不同的perl版本，将Perl的lib目录放在perl-5.10目录下，同时在解释器初始化时候创建一个环境变量PERL5LIB，并设置为perl-5.10\lib目录，如果要支持别的版本，可以创建一个新的perl目录。
2、Perl编译的头文件和lib库在perl-5.10\lib\CORE目录下，如果要支持别的版本，可以修改PERL_LIB_PATH的定义以及工程参数中的头文件、依赖库路径。