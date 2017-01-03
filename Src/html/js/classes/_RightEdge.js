/*******************************************\
  游戏人对联广告类(2006-06-14)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.RightEdge = function (uid)
{
	this.obj = document.getElementById(uid);
	this.oTop = this.obj.offsetTop;
	this.start = function ()
	{
		var timeout,oldTop,newTop,topC,obj,m;
		timeout = 500;
		oldTop = parseInt(this.obj.style.top,10);
		newTop = document.body.scrollTop + this.oTop;
		topC = Math.ceil(Math.abs(newTop-oldTop)/20);
		if(newTop != oldTop)
		{
			this.obj.style.top = oldTop + topC * (newTop>oldTop?1:-1);
			timeout = 10;
		}
		m = this;
		setTimeout(function(){m.start()},this.timeout);
	}
	this.start();
}