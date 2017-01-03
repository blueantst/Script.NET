/*******************************************\
  Function 的扩展方法(2006-8-25)
  部分程序收集于 Internet
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/

// IE5 的 apply 和 call
if (typeof(Function.prototype.apply)!="function")
{
	Function.prototype.apply = function (obj, argu)
	{ 
		if (obj) obj.constructor.prototype._caller = this;  
		var argus = new Array(); 
		for (var i=0;i<argu.length;i++) argus[i] = "argu[" + i + "]"; 
		var r; 
		eval("r = " + (obj ? ("obj._caller(" + argus.join(",") + ");") : ("this(" + argus.join(",") + ");"))); 
		return r; 
	};
	Function.prototype.call = function (obj)
	{
		var argu = new Array(); 
		for (var i=1;i<arguments.length;i++) argu[i-1] = arguments[i];
		return this.apply(obj, argu);
	};
}