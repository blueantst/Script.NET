/********************************************************************************\
 * 类 名 称： DragDrop | 代码来自 http://blog.km0ti0n.be/js/
 * 功能说明： 可拖动类 
 * 版权信息： CopyRight 2005-2006 JoeCom 
 * 创 建 人： JoeCom | MSN:juwuyi@hotmail.com | blog:http://hi.baidu.com/joecom 
 * 创建日期： 2006-07-19 
 * 其它说明： 此类并非Dron原创，这里改成DronFw所用的类的形式，望原作者莫怪。
 *            Dron 修改部分：增加限定拖动范围的方法
\********************************************************************************/
DronFw.Class.DragDrop = function(obj)
{
    var me = this;
    this.foo = (typeof obj=="string")?document.getElementById(obj):obj;
    this.foo.onmousedown = function(e)
	{
		var foo = me.foo;
		e = e||event;
		foo.oOffset = (e.layerX)?{x:e.layerX,y:e.layerY}:{x:e.offsetX,y:e.offsetY};
		document.onmousemove = me.drag;
		document.onmouseup   = me.drop;
		document.onselectstart = function(){return false;};
    }
    this.drag = function(e)
	{
		var foo = me.foo;
		e=e||event;
		var top = e.clientY - foo.oOffset.y + document.body.scrollTop;
		var left = e.clientX - foo.oOffset.x + document.body.scrollLeft;
		if (me.range)
		{
			top = (top<me.range.y1) ? (me.range.y1) : ((top>me.range.y2-foo.offsetHeight) ? (me.range.y2-foo.offsetHeight) : top);
			left = (left<me.range.x1) ? (me.range.x1) : ((left>me.range.x2-foo.offsetWidth) ? (me.range.x2-foo.offsetWidth) : left);
		}
		foo.style.top  = top + "px";
		foo.style.left = left + "px";
    }
    this.drop = function()
	{
		document.onmousemove = document.onmouseup = document.onselectstart = null;
    }
	this.setRange = function(xx1,yy1,xx2,yy2)
	{
		me.range = {x1:xx1,x2:xx2,y1:yy1,y2:yy2};
	}
}