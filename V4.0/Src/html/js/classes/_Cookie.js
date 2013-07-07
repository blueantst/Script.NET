/*******************************************\
  游戏人Cookie操作类(2006-07-21)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.Cookie = function ()
{
	this.get = function (Name)
	{
		var search = Name + "=";
		if(document.cookie.length > 0)
		{
			var offset = document.cookie.indexOf(search);
			if(offset != -1)
			{
				offset += search.length;
				var end = document.cookie.indexOf(";", offset);
				if(end == -1) end = document.cookie.length;
				return unescape(document.cookie.substring(offset, end));
			}
			else return "";
		}
		else return "";
	}
	this.set = function(name,value)
	{
		var today = new Date();
		var expires = new Date();
		expires.setTime(today.getTime() + 1000*60*60*24*365);
		document.cookie = name + "=" + escape(value) + ";path=/"+"; expires=" + expires.toGMTString();
	}
	var o = this;
	this.del = function (name)
	{
		o.set(name,"");
	}
}