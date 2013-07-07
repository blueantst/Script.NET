/*******************************************\
  Array 的扩展方法(2006-8-8)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
// 自定义数组删除
Array.prototype.del = function(n)
{
	if (n<0) return this;
	return this.slice(0,n).concat(this.slice(n+1,this.length));
}
// 数组洗牌
Array.prototype.random = function()
{
	var nr=[], me=this, t;
	while(me.length>0)
	{
		nr[nr.length] = me[t = Math.floor(Math.random() * me.length)];
		me = me.del(t);
	}
	return nr;
}
// 数字数组排序
Array.prototype.sortNum = function(f)
{
	if (!f) f=0;
	if (f==1) return this.sort(function(a,b){return b-a;});
	return this.sort(function(a,b){return a-b;});
}
// 获得数字数组的最大项
Array.prototype.getMax = function()
{
	return this.sortNum(1)[0];
}
// 获得数字数组的最小项
Array.prototype.getMin = function()
{
	return this.sortNum(0)[0];
}
// 数组第一次出现指定元素值的位置
Array.prototype.indexOf = function(o)
{
	for (var i=0; i<this.length; i++) if (this[i]==o) return i;
	return -1;
}
// 移除数组中重复的项
Array.prototype.removeRepeat=function()
{
	this.sort();
	var rs = [];
	var cr = false;
	for (var i=0; i<this.length; i++)
	{
		if (!cr) cr = this[i];
		else if (cr==this[i]) rs[rs.length] = i;
		else cr = this[i];
	}
	var re = this;
	for (var i=rs.length-1; i>=0; i--) re = re.del(rs[i]);
	return re;
}