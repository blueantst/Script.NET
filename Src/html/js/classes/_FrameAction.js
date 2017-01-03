/*******************************************\
  游戏人帧动作类(2006-7-26)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.FrameAction = function (timer)
{
	var me = this;
	this.Timer = (typeof(timer)=="number") ? timer : 80;
	this.load = function (commandArray)
	{
		this.commandArray = commandArray;
	}
	this.start = function (n)
	{
		(typeof(n)=="number") || (n=0);
		if(n>=this.commandArray.length)return;
		eval(this.commandArray[n]);
		setTimeout(function(){me.start(++n)},this.Timer);
	}
}