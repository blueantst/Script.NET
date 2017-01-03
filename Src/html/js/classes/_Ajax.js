/*******************************************\
  游戏人 Ajax 类(2006-7-30)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.Ajax = function ()
{
	var navName = (function ()
	{
		var u = navigator.userAgent.toLowerCase();
		if (/gecko/i.test(u)) return "moz";
		if (/msie/i.test(u)) return "ie";
		return "other";
	})();
	var ie = navName=="ie";
	var moz = navName=="moz";

	// xmlHttp对象
	this.xmlHttp = function ()
	{
		if (ie)
		{
			for (var i=0; i<2; i++)
			try
			{
				var r = new ActiveXObject(["MSXML2.XMLHTTP", "Microsoft.XMLHTTP"][i]);
				return r;
			}
			catch (e) { return null;};
		}
		else return new XMLHttpRequest();
	}

	// xmlDom对象
	this.xmlDom = function ()
	{
		if (ie)
			for (var i=0; i<4; i++)
				try
				{
					var r = new ActiveXObject(["MSXML2.DOMDocument", "Microsoft.XMLDOM", "MSXML.DOMDocument", "MSXML3.DOMDocument"][i]);
					return r;
				}
				catch (e){return null;}
		else return document.implementation.createDocument("", "doc", null);
	}

	// 装载一个Http
	this.loadHttp = function (url, asy, fun)
	{
		var xmlHttp = this.xmlHttp();
		xmlHttp.open("GET", url, (asy ? true : false));
		if (asy)
		{
			xmlHttp.onreadystatechange = function()
			{
				if(xmlHttp.readyState==4) fun (xmlHttp);
			};
			xmlHttp.send(null);
		}
		else
		{
			try
			{
				xmlHttp.send(null);
			}
			catch (e)
			{
				throw new Error("LoadHttp Error.");
				return;
			}
			return xmlHttp.responseText;
		}
	}

	// 装载一个XmlDom
	this.loadXml = function (url, asy, fun)
	{
		var xmlDom = this.xmlDom();
		xmlDom.async = asy ? true : false;
		if (asy)
		{
			if (ie) xmlDom.onreadystatechange = function ()
			{
				if(xmlDom.readyState == 4) fun(xmlDom);
			}
			else xmlDom.onload = function ()
			{
				fun(xmlDom);
			}
		}
		xmlDom.load(url);
		return xmlDom;
	}
	// 二进制转换
	this.Bin2Str = function (binary)
	{
		var rec = new ActiveXObject("ADODB.RecordSet");
		rec.Fields.Append("DDD",201,1);
		rec.open();
		rec.addNew();
		rec(0).appendChunk(binary);
		rec.update();
		var result = rec(0).value;
		rec.Close();
		return result;
	}
	// 取得xmlDom对象的xml内容
	this.getXml = function (xDom)
	{
		if (ie) return xDom.xml;
		else return (new XMLSerializer()).serializeToString(xDom);
	}
}