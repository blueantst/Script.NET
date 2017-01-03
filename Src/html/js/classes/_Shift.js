/*******************************************\
  游戏人移位类(2006-7-26)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.Shift = function (Obj)
{
	var me = this;
	if (typeof (Obj)=="string") this.Obj = document.getElementById(Obj);
	else this.Obj = Obj;
	this.Timer = 10;
	this.Move = function (x2,y2,fun)
	{
		var x1 = parseInt(this.Obj.style.left,10);
		var y1 = parseInt(this.Obj.style.top,10);
		if ((x1==x2)&&(y1==y2))
		{
			if (fun) return fun();
		}
		else
		{
			var xc = Math.ceil(Math.abs(x1-x2)/20);
			var yc = Math.ceil(Math.abs(y1-y2)/20);
			this.Obj.style.left = x1 + xc * (x1<x2?1:-1) + "px";
			this.Obj.style.top = y1 + yc * (y1<y2?1:-1) + "px";
			setTimeout(function (){me.Move(x2,y2,fun)},me.Timer);
		}
	}
}