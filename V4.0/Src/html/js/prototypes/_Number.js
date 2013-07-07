/*******************************************\
  Number 的扩展方法(2006-8-8)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
// 数字补零
Number.prototype.fillZero = function (len)
{
	var s = this.toString(10);
	while (s.length<len) s = "0" + s;
	return s;
}