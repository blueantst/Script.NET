/************************************************************
 *           tree.js                                         *
 *           copyright Simon Norris 2002                     *
 *           email: si@web-sn.com                            *
 *           EditAce home page: http://www.web-sn.com        *
 *                                                           *
 *           You may use or modify this script for           *
 *           non-profit sites provided this copyright        *
 *           notice remains unchanged.                       *
 ************************************************************/
var contextRoot ="/code/html/"

function web_showhideAll() {
    if (document.getElementById('treecontainer').style.display == "block") {
        document.getElementById('treecontainer').style.display = "none";
    }
    else {
        document.getElementById('treecontainer').style.display = "block";
    }
}



function web_toggleImgs(el,rid) {
    var allImg = document.body.getElementsByTagName("IMG");
    for (i=0; i<allImg.length; i++) {
        if (allImg[i].className == "fldricon") {
          		
        	
	            var imgSrc = allImg[i].src;
	            if(imgSrc.indexOf('folder') != -1)
	            {
	                allImg[i].src = "images/folder_2.gif";
	                
	            }
	            else
	            {
	                allImg[i].src = "images/fldr_closed.gif";
	               
	            }
            
          	
            
        }
    }
    if(el.src.indexOf('folder') != -1)
    {
        el.src = "images/folder_1.gif";
        
    }
    else
    {
        el.src = "images/fldr_open.gif";
        
    }
}



function showhidesub(targetEl) {       
//    var chk = srcEl.src;
    if (document.getElementById(targetEl).style.display == 'none') {
        document.getElementById(targetEl).style.display = 'block';

//        if (chk.indexOf('middle') == -1) {
//            srcEl.src = "images/bottom_minus.gif";
//        }
//        else {
//            srcEl.src = "images/middle_minus.gif";
//        }

    }
    else {
        document.getElementById(targetEl).style.display = 'none';

//        if (chk.indexOf('middle') == -1) {
//            srcEl.src = "images/bottom_plus.gif";
//        }
//        else {
//            srcEl.src = "images/middle_plus.gif";
//        }

    }
    
}

function showhidesub(targetEl,rid) {       
//    var chk = srcEl.src;
    if (document.getElementById(targetEl).style.display == 'none') {
        document.getElementById(targetEl).style.display = 'block';

//        if (chk.indexOf('middle') == -1) {
//            srcEl.src = "images/bottom_minus.gif";
//        }
//        else {
//            srcEl.src = "images/middle_minus.gif";
//        }

    //ch=rid.substring(0,rid.indexOf("Link"));
    //ch="fldrImg"+ch;
    //document.getElementById("fldrImgstandby").src="images/folder_1.gif";
 

    }
    else {
        document.getElementById(targetEl).style.display = 'none';

//        if (chk.indexOf('middle') == -1) {
//            srcEl.src = "images/bottom_plus.gif";
//        }
//        else {
//            srcEl.src = "images/middle_plus.gif";
//        }

    //ch=rid.substring(0,rid.indexOf("Link"));
    //ch="fldrImg"+ch;
    //document.getElementById("fldrImgstandby").src="images/folder_2.gif";

    }
    
}




function setHref(str)
{
    //get element href string
    var iHref = "javascript:newWin('tree/" + str + "_create.screen');";
    var qHref = "tree/" + str + "_query.screen";
    var dHref = "javascript:delconfirm();";


   /*
    var browserName = navigator.appName;
    if(browserName.toLowerCase().search("netscape") > -1)
    {
        var leftW = (screen.width - 600) / 2;
	var topH = (screen.height - 300) / 2;
	iHref = "javascript:window.open('tree/" + str +
	    "_create.screen', 'insertwin', 'menubar=no, resizable=yes, " +
	    "left=" + leftW + ",top=" + topH + ",width=600,height=300').fucus();";
    }

    //get frames id
    var bId = parent.frames["bottom"];

    //get anchor id
    var insertId = bId.document.getElementById("insert_href");
    var queryId = bId.document.getElementById("query_href");
    var deleteId = bId.document.getElementById("delete_href");
    var helpId = bId.document.getElementById("help_href");

    showBottom(insertId,queryId,deleteId);

    str=getBasicName(str);
    var hHref = "javascript:displayHelp('" + str + "')";

    //set anchor href attribute
    insertId.href = iHref;
    queryId.href = qHref;
    deleteId.href = dHref;
    helpId.href = hHref;

	*/
}

/**
 * add this method in 2002-12-27
 * get Help id of Basic Table
 * @param str is Basic Table Name
 * @return page name which be defined in resource file
 */
function getBasicName(str)
{
	if(str=="feeclassdescription")
	{
		return "charge.basetbl.calltype";
	}
	if(str=="currencydescription")
	{
		return "charge.basetbl.currecytype";
	}
	if(str=="chargeruledescription")
	{
		return "charge.basetbl.chargerule";
	}
	if(str=="mediatypedescription")
	{
		return "charge.basetbl.mediatype";
	}
	return "charge.draft."+str;
}

/**
 * add this method in 2002-11-20 11:39 by zhsjun
 * this method modify the href attribute of the anchor tag in the act1.htm file
 * the query_href is query anchor id
 */
function setQHref(str, listType)
{
    //get element href string
    var qHref = "tree/" + str + "_query.screen?listType="+listType;

    //get frames id
    var bId = parent.frames["bottom"];

    //get anchor id
    var queryId = bId.document.getElementById("query_href");

    //set anchor href attribute
    queryId.href = qHref;
    queryId.style.visibility = "visible";
}

/**
 * add this method in 2002-11-16 by sun
 * this method hidden the href of the anchor tag in the act.htm file
 * the insert_href is insert anchor id
 * the query_href is query anchor id
 * the delete_href is delete anchor id
 */
function hiddenBottom(str1,str2,str3)
{
    try
    {
        //get frames id
        var bId = parent.frames["bottom"];

        //get anchor id
        var insertId = bId.document.getElementById("insert_href");
        var queryId = bId.document.getElementById("query_href");
        var deleteId = bId.document.getElementById("delete_href");
        var helpId = bId.document.getElementById("help_href");

        if (str1 == 1)
        {
            insertId.style.visibility = "hidden";
        }
        if (str2 == 1)
        {
            queryId.style.visibility = "hidden";
        }
        if (str3 == 1)
        {
            deleteId.style.visibility = "hidden";
        }
        if(helpId != null && str2 == 1)
        {
        	helpId.style.visibility = "hidden";
        }
    }
    catch(e)
    {}
}

/**
 * add this method in 2002-11-16 by sun
 * this method show the href of the anchor tag in the act.htm file
 * the insert_href is insert anchor id
 * the query_href is query anchor id
 * the delete_href is delete anchor id
 */
function showBottom(str1,str2,str3)
{
    //get frames id
    var bId = parent.frames["bottom"];
    var helpId = bId.document.getElementById("help_href");
    if(helpId != null)helpId.style.visibility = "visible";

    str1.style.visibility = "visible";
    str2.style.visibility = "visible";
    str3.style.visibility = "visible";
}