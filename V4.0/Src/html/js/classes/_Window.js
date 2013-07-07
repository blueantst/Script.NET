/*******************************************\
  游戏人窗体类(2006-07-25)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.Window = function ()
{
	var Me = this;
	var div = document.createElement("div");
	this.StyleName = "normal";
	this.Left = 0;
	this.Top = 0;
	this.Width = 300;
	this.Height = 200;
	this.Title = "Undefine Title";
	this.Content = "&nbsp;";
	this.Ico = DronFw.Path + "classes/_Window/normal/ico.gif";
	this.MinButton = true;
	this.MaxButton = true;
	this.CloButton = true;
	//系统属性
	this.isMaxStatus = false;
	this.dragStatus = false;
	this.SetDivId = function(id)
	{
		div.id = id;
	}
    this.Drag = function(e)
	{
		e = e || event;
		div.style.left = (Me.Left=e.clientX-Me.chaLeft) + "px";
		div.style.top = (Me.Top=e.clientY-Me.chaTop) + "px";
    }
    this.Drop = function()
	{
		document.onmousemove = document.onmouseup = document.onselectstart = null;
    }
	this.Show = function ()
	{
		var IcoFile = DronFw.Path + "classes/_Window/" +this.StyleName + "/ico.gif";
		var s = "<table width=\"100%\" height=\"100%\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\" class=\"window_table\">\
			<tr bgcolor=\"#08246b\">\
				<td width=\"18\" height=\"18\" align=\"center\"><img src=\"" + IcoFile + "\" class=\"window_ico\"></td>\
				<td class=\"window_title\">\
					" + this.Title + "\
				</td>\
				<td width=\"48\" class=\"window_button\" align=\"right\"><input type=\"button\" class=\"window_button1\"" +((Me.MinButton)?"":" style=\"display:none\"")+ "><input type=\"button\" class=\"window_button2\"" +((Me.MaxButton)?"":" style=\"display:none\"")+ "><input type=\"button\" class=\"window_button3\"" +((Me.CloButton)?"":" style=\"display:none\"")+ ">\
				</td>\
			</tr>\
			<tr>\
				<td colspan=\"3\" align=\"left\" valign=\"top\" class=\"window_content\">\
					" + this.Content + "\
				</td>\
			</tr>\
		</table>";
		div.className = "DronFw_Window";
		div.style.left = this.Left + "px";
		div.style.top = this.Top + "px";
		div.style.width = this.Width + "px";
		div.style.height = this.Height + "px";
		div.style.zIndex = ++window._DronFw_Window_zIndex;

		document.body.appendChild(div);
		(this.Window=div).innerHTML = s;
		var inputs = div.getElementsByTagName("input");
		var contable = div.getElementsByTagName("table")[0];
		var contd = contable.rows[1].cells[0];
		var titbar = contable.rows[0].cells[1];
		inputs[0].onclick = function ()
		{
			if(!document.all)
			{
				window.alert("Can't Compatible!");
				return this.style.display = "none";
			}
			contd.style.display = contd.style.display=="none" ? "block" : "none";
			div.style.height = contd.style.display=="none" ? "18px" : (Me.Height+"px");
		}
		titbar.ondblclick = inputs[1].onclick = function ()
		{
			Me.isMaxStatus = ! Me.isMaxStatus;
			if (Me.isMaxStatus)
			{
				div.style.left = 0 + "px";
				div.style.top = 0 + "px";
				div.style.width = document.body.clientWidth + "px";
				div.style.height = document.body.clientHeight + "px";
			}
			else
			{
				div.style.left = Me.Left + "px";
				div.style.top = Me.Top + "px";
				div.style.width = Me.Width + "px";
				div.style.height = Me.Height + "px";				
			}
		}
		inputs[2].onclick = function ()
		{
			document.body.removeChild(div);
			delete Me;
		}
		titbar.onmousedown = function (e)
		{
			e = e || event;
			Me.dragStatus = true;
			Me.chaLeft = e.clientX - Me.Left;
			Me.chaTop = e.clientY - Me.Top;
			document.onmousemove = Me.Drag;
			document.onmouseup   = Me.Drop;
			document.onselectstart = function(){return false;};
		};
		(div.onclick = function () {
			div.style.zIndex = ++window._DronFw_Window_zIndex;
			if (window._DronFw_Window_currentWindow!="w")
			{
				var contable = window._DronFw_Window_currentWindow.getElementsByTagName("table")[0];
				contable.rows[0].style.backgroundColor = "#9C9A9C";
			}			
			var contable = div.getElementsByTagName("table")[0];
			contable.rows[0].style.backgroundColor = "#08246b";
			window._DronFw_Window_currentWindow = div;
		})();
	}
};
(
	function ()
	{
		var head = document.getElementsByTagName("head")[0];
		var link = document.createElement("link");
		link.rel = "stylesheet";
		link.href = DronFw.Path + "classes/_Window/normal/win.css";
		link.type = "text/css";
		link.media = "all";
		head.appendChild(link);
		if (!window._DronFw_Window_zIndex) window._DronFw_Window_zIndex = 1000;
		if (!window._DronFw_Window_currentWindow) window._DronFw_Window_currentWindow = "w";
	}
)();