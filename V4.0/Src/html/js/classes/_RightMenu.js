/*******************************************\
  游戏人右键菜单类(2006-7-28)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.RightMenu = function ()
{
	//限定不能复用
	if (window._DronFw_RightMenu_Status) return ;
	window._DronFw_RightMenu_Status = true;

	this.MenuContent = [];
	this.AddItem = function (nam,url)
	{
		this.MenuContent[this.MenuContent.length] = [nam,url];
	}
	this.AddLine = function ()
	{
		this.MenuContent[this.MenuContent.length] = "line";
	}
	this.Setup = function ()
	{
		var div = document.createElement("div");
		with (div.style)
		{
			position = "absolute";
			left = top = "0px";
			width = "150px";
			lineHeight = "20px";
			backgroundColor = "#d4d0c8";
			borderTop = "2px outset #fff";
			borderLeft = "2px outset #fff";
			borderBottom = "2px outset #969696";
			borderRight = "2px outset #969696";
			cursor = "default";
			zIndex = 2000;
		}
		document.body.appendChild(div);
		var s = "";
		for (var i=0; i<this.MenuContent.length; i++)
		if (this.MenuContent[i]=="line") s += "<div style=\"height:0px;line-height:0px;overflow:hidden;border-top:1px solid #888;border-bottom:1px solid #fff;\"></div>"
		else s += "<div style=\"width:100%;\" onclick=\"" + this.MenuContent[i][1] + "\" onmouseover=\"style.background=\'#0a246a\';style.color=\'#fff\';\" onmouseout=\"style.background=\'\';style.color=\'\';\">&nbsp;" + this.MenuContent[i][0] + "</div>";
		div.innerHTML = s;
		div.oW = div.offsetWidth;
		div.oH = div.offsetHeight;
		div.style.display = "none";
		document.oncontextmenu = function (e)
		{
			var e = e||event;
			var nx = document.body.scrollLeft + e.clientX
			var ny = document.body.scrollTop + e.clientY;
			if (e.clientX + div.oW + 20> document.body.offsetWidth)nx = nx - div.oW;
			if (e.clientY + div.oH + 20> document.body.offsetHeight)ny = ny - div.oH;
			div.style.left = nx;
			div.style.top = ny;
			div.style.display = "block";
			this.onclick = function ()
			{
				setTimeout(function ()
				{
					div.style.display = "none";
				},100);
				this.onclick = null;
			}
			return false;
		}
	}
}