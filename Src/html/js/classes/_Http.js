/*******************************************\
  游戏人超级简单的Http类(2006-07-24)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
if (document.all)
with (document)
	write("\
	<script type=\"text/vbscript\">\r\n\
	Function bytes2BSTR(vIn)\r\n\
		strReturn = \"\"\r\n\
		For i = 1 To LenB(vIn)\r\n\
			ThisCharCode = AscB(MidB(vIn,i,1))\r\n\
			If ThisCharCode < &H80 Then\r\n\
				strReturn = strReturn & Chr(ThisCharCode)\r\n\
			Else\r\n\
				NextCharCode = AscB(MidB(vIn,i+1,1))\r\n\
				strReturn = strReturn & Chr(CLng(ThisCharCode) * &H100 + CInt(NextCharCode))\r\n\
				i = i + 1\r\n\
			End If\r\n\
		Next\r\n\
		bytes2BSTR = strReturn\r\n\
	End Function\r\n\
	</script>\
	");
DronFw.Class.Http = function ()
{
	this.get = function (url,l,s)
	{
		var Http = (document.all) ? (new ActiveXObject("Microsoft.XMLHTTP")) : (new XMLHttpRequest());
		(typeof(l)=="string") || (l="utf-8");
		(l.toLowerCase()=="gb2312") || (l.toLowerCase()=="utf-8") || (l="utf-8");
		(typeof(s)=="boolean") || (s=false);
		Http.open("get",url,s);
		Http.send(null);
		if (document.all) if (l.toLowerCase()=="gb2312") return s ? Http : bytes2BSTR(Http.responseBody);
		return s ? Http : Http.responseText;
	}
}