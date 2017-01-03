/*******************************************\
  游戏人样式表操作类(2006-7-30)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.CSS = function ()
{
	var head = document.getElementsByTagName("head")[0];
	this.Append = function (url, tit)
	{
		var link = document.createElement("link");
		link.rel = "stylesheet";
		link.href = url;
		link.type = "text/css";
		link.media = "all";
		if (tit) link.title = tit;
		if (head) head.appendChild(link);
		else document.documentElement.appendChild(link);
		return link;
	}
	this.Remove = function (obj)
	{
		if (typeof(obj)=="object" && obj.rel == "stylesheet")
		{
			obj.rel = "NoCss";
			if (head) head.removeChild(obj);
			else document.documentElement.removeChild(obj);
			return true;
		}
		return false;
	}
	this.RemoveAll = function ()
	{
		if (!document.all) return window.alert("此功能暂不兼容非IE浏览器，请稍候...");
		var link = document.getElementsByTagName("link");
		var styles = document.getElementsByTagName("style");
		for (var i in link) if (link[i].rel=="stylesheet") link[i].type = "NoCss";
		for (var i in styles) styles[i].type = "NoCss";
		for (var i=0; i<document.all.length; i++) document.all[i].style.cssText="";
	}
	this.ApplySkin = function (skinName)
	{
		if (!document.all) return window.alert("此功能暂不兼容非IE浏览器，请稍候...");
		var link = document.getElementsByTagName("link"),t;
		for (var i in link)
		if (t=link[i].title)
		{
			if (t==skinName)
			{
				link[i].rel = "stylesheet";
				link[i].type = "text/css";
			}
			else
			{
				link[i].rel = "No Money";//alternate stylesheet
				link[i].type = "No OK!";
			}
		};
	}
}