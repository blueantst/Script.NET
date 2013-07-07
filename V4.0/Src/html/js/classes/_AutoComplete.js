/*******************************************\
  游戏人自动完成类(2006-7-27)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.AutoComplete = function ()
{
	this.Apply = function (obj,autolist,al)
	{
		if (typeof (obj)=="string") obj = document.getElementById(obj);
		al = al ? true : false;

		var div = document.createElement("div");
		div.selectbar = "true";
		div.style.display = "none";
		div.style.position = "absolute";
		div.style.left = obj.offsetLeft + "px";
		div.style.top = obj.offsetTop + obj.offsetHeight + "px";
		div.style.width = obj.offsetWidth + "px";
		div.style.border = "1px solid #000";
		div.style.backgroundColor = "#fff";
		div.style.lineHeight = "15px";
		div.style.cursor = "default";
		document.body.appendChild(div);

		var sdiv=[],acdiv=-1;
		obj.onkeyup = function (e)
		{
			var e = e||event;
			if (e.keyCode==38||e.keyCode==40||e.keyCode==13) return false;
			var s = "";
			for (var i=0; i<autolist.length; i++)
			if (al ? (autolist[i].indexOf(this.value)!=-1) : (autolist[i].indexOf(this.value)==0))
			s += "<div style=\"width:100%;\" onmouseover=\"style.background=\'#009\';style.color=\'#fff\'\" onmouseout=\"style.background=\'#fff\';style.color=\'#000\'\">" + autolist[i] + "</div>";
			div.innerHTML = s;
			acdiv=-1;
			sdiv = div.getElementsByTagName("div");
			if (sdiv.length>10) {div.style.height="150px";div.style.overflow="hidden";}
			else {div.style.height=sdiv.length * 15 + "px";}
			div.style.display = (s=="")?"none":"block";
		}
		obj.onkeydown = function (e)
		{
			var e = e||event;
			switch (e.keyCode)
			{
				case 38:
					if (acdiv-1<0) return;
					if (sdiv[acdiv]) {sdiv[acdiv].style.background = "#fff";sdiv[acdiv].style.color="#000";}
					if (sdiv[acdiv-1]) {sdiv[--acdiv].style.background = "#009";sdiv[acdiv].style.color="#fff";}
					if (acdiv>9) div.scrollTop = acdiv * 15; else div.scrollTop=0;
					break;
				case 40:
					if (acdiv+1>=sdiv.length) return;
					if (sdiv[acdiv]) {sdiv[acdiv].style.background = "#fff";sdiv[acdiv].style.color="#000";}
					if (sdiv[acdiv+1]) {sdiv[++acdiv].style.background = "#009";sdiv[acdiv].style.color="#fff";}
					if (acdiv>9) div.scrollTop = acdiv * 15; else div.scrollTop=0;
					break;
				case 13:
					if (sdiv[acdiv]) obj.value = sdiv[acdiv].innerHTML;
					div.style.display = "none";
					break;
			}
		}
		obj.onblur = function () {
			if (document.activeElement && document.activeElement.selectbar=="true") return ;
			setTimeout(function (){div.style.display = "none"},100);
		}
		div.onmousedown = function (e)
		{
			var e = e||event;
			var srcElement = e.srcElement || e.target;
			if (srcElement.selectbar) return;
			obj.value = srcElement.innerHTML;
			div.style.display = "none";
		}
	}
}