/*******************************************\
  String 的扩展方法(2006-8-8)
  本文档原作者：zmm，后期修改与补充：dron
\*******************************************/
// 合并多个空白为一个空白
String.prototype.resetBlank = function()
{
	return this.replace(/\s+/g," ");
}
// 除去左边空白
String.prototype.LTrim = function()
{
	return this.replace(/^\s+/g,""); 
} 
// 除去右边空白
String.prototype.RTrim = function()
{
	return this.replace(/\s+$/g,""); 
}
// 除去两边空白
String.prototype.trim = function()
{
	return this.replace(/(^\s+)|(\s+$)/g,""); 
}
// 保留数字
String.prototype.getNum = function()
{
	return this.replace(/[^\d]/g,"");
}
// 保留字母
String.prototype.getEn = function()
{
	return this.replace(/[^A-Za-z]/g,""); 
}
// 保留中文
String.prototype.getCn = function()
{
	return this.replace(/[^\u4e00-\u9fa5\uf900-\ufa2d]/g,"");
}
// 得到字节长度
String.prototype.getRealLength = function()
{
	return this.replace(/[^\x00-\xff]/g,"--").length;
}
// 从左截取指定长度的字串
String.prototype.left = function(n)
{
	return this.slice(0,n);
}
// 从右截取指定长度的字串
String.prototype.right = function(n)
{
	return this.slice(this.length-n);
}
// HTML编码
String.prototype.HTMLEncode = function()
{
	var re = this;
	var q1 = [/\x26/g,/\x3C/g,/\x3E/g,/\x20/g];
	var q2 = ["&amp;","&lt;","&gt;","&nbsp;"];
	for(var i=0;i<q1.length;i++)
	re = re.replace(q1[i],q2[i]);
	return re;
}
// Unicode转化
String.prototype.ascW = function()
{
	var strText = "";
	for (var i=0; i<this.length; i++) strText += "&#" + this.charCodeAt(i) + ";";
	return strText;
}