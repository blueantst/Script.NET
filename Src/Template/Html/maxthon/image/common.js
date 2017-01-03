function el(id) {
	return document.getElementById(id);
}

function checkPassword(pwd){

  var objLow=document.getElementById("pwdLow");
  var objMed=document.getElementById("pwdMed");
  var objHi=document.getElementById("pwdHi");

  objLow.className="pwd-strength-box";
  objMed.className="pwd-strength-box";
  objHi.className="pwd-strength-box";

  if(pwd.length<6){
    objLow.className="pwd-strength-box-low";
  }else{
    var p1= (pwd.search(/[a-zA-Z]/)!=-1) ? 1 : 0;
    var p2= (pwd.search(/[0-9]/)!=-1) ? 1 : 0;
    var p3= (pwd.search(/[^A-Za-z0-9_]/)!=-1) ? 1 : 0;
    var pa=p1+p2+p3;
    
    if(pa==1){
      objLow.className="pwd-strength-box-low";
    }else if(pa==2){
      objMed.className="pwd-strength-box-med";
    }else if(pa==3){
      objHi.className="pwd-strength-box-hi";
    }
  }
}

function isEmail(s) {   
   if (s.length > 100)   
       return   false;   
   var regu="^(([0-9a-zA-Z]+)|([0-9a-zA-Z]+[_.0-9a-zA-Z-]*[_.0-9a-zA-Z]+))@([a-zA-Z0-9-]+[.])+([a-zA-Z]{2}|net|NET|com|COM|gov|GOV|mil|MIL|org|ORG|edu|EDU|int|INT)$";       
   var re = new RegExp(regu);   
   if (s.search(re) != -1)   
      return true;
   else     
      return false;
}

String.prototype.trim  = function(){return this.replace(/^\s+|\s+$/g,"");}

function create_request() {
	if (window.ActiveXObject) {
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	} else if (window.XMLHttpRequest) {
		xmlhttp = new XMLHttpRequest();
	}	
	return xmlhttp;
}


var sign_up_mgr = {
	xmlhttp: null,
	pwd1: null,
	pwd2: null,
	dir: "",
		
	request_nickname: function(id) {
		el("spn_nickname_error").style.display = "none";
		this.xmlhttp = create_request();
		this.xmlhttp.onreadystatechange = this.request_nickname_callback;
		var v = id.value.trim();
		if (v.length == 0) return;
		var url = this.dir + "ajax/sign-up.ashx?name=" + v;
		this.xmlhttp.open("GET", url, true);
		this.xmlhttp.send(null);
	},
	
	request_nickname_callback: function() {
		if (sign_up_mgr.xmlhttp.readyState == 4) {
			if (sign_up_mgr.xmlhttp.status == 200) {
				var text = sign_up_mgr.xmlhttp.responseText;
				if (text == "1") {
					el("spn_nickname_error").style.display = "block";
				}
			}
		}	
	},
	
	request_account: function(id) {
		el("spn_account_error").style.display = "none";
		this.xmlhttp = create_request();
		this.xmlhttp.onreadystatechange = this.request_account_callback;
		var v = id.value.trim();
		if (v.length == 0) return;
		var url = this.dir + "ajax/sign-up.ashx?email=" + v;
		this.xmlhttp.open("GET", url, true);
		this.xmlhttp.send(null);
	},
	
	request_account_callback: function() {
		if (sign_up_mgr.xmlhttp.readyState == 4) {
			if (sign_up_mgr.xmlhttp.status == 200) {
				var text = sign_up_mgr.xmlhttp.responseText;
				if (text == "2") {
					el("spn_account_error").style.display = "block";
				}
			}
		}	
	},
	
	check_pwd1: function(pwd) {
		this.pwd1 = pwd;
		if (this.pwd1 == null || this.pwd2 == null) return;
		var spn = el("spn_pwd_error");
		spn.style.display = "none";
		if (this.pwd1.value.length == 0 || this.pwd2.value.length == 0)
			return;	
		if (this.pwd1.value != this.pwd2.value)
			spn.style.display = "block";
	},
	
	check_pwd2: function(pwd) {
		this.pwd2 = pwd;
		if (this.pwd1 == null || this.pwd2 == null) return;
		var spn = el("spn_pwd_error");
		spn.style.display = "none";
		if (this.pwd1.value.length == 0 || this.pwd2.value.length == 0)
			return;		
		if (this.pwd1.value != this.pwd2.value)
			spn.style.display = "block";
	}	
}

var fb_mgr = {
	xmlhttp: null,
	account: null,
	content: null,
	innerAccount:null,
	innerContent:null,
	
    showDialog : function() {
        tb_show("","#TB_inline?height=250&width=400&inlineId=feedback_div",false);
        if (window.navigator.userAgent.indexOf("Firefox")>=1){
        	if(this.account != null)
				el('txtAccount').value = this.account;
        }
    },
    
    showLoading : function() {
		tb_show("","#TB_inline?height=240&width=400&inlineId=feedback_loading",false);
    },    
    
    showComplete : function() {
		tb_show("","#TB_inline?height=240&width=400&inlineId=send_over",false);
    },
    
    getAccountValue : function(account) {
		el('send_error').style.display='none';
		this.account = account.trim();
    },   
        
    getContentValue : function(content) {
		el('send_error').style.display='none';
		this.content = content.trim();
    },  
    
    sendmail : function(register) {
		if (window.navigator.userAgent.indexOf("MSIE")>=1){

			if(this.account == null) {
				if(register != null) {
					this.account = register;
				}
			}
			
			if(this.account != null)
				el('txtAccount').value = this.account;	
			if(this.content != null)
				el('txtContent').value = this.content;		

			if (this.account == null || this.content == null || this.account.length == 0 || this.content.length == 0 || isEmail(this.account) == false) {
				el('send_error').style.display='none';
				el('alert_message').style.display = 'block';
				this.showDialog();
				return;
			}
			
		}
		else
		{
			if(this.account == null) {
				if(register != null) {
					this.account = register;
				}
			}

			if (this.account == null || this.content == null || this.account.length == 0 || this.content.length == 0 || isEmail(this.account) == false) {
				this.showDialog();
				el('send_error').style.display='none';
				el('alert_message').style.display = 'block';
				if(this.account != null)
					el('txtAccount').value = this.account;	
				if(this.content != null)
					el('txtContent').value = this.content;	
				this.showDialog();
				if(this.account != null)
					el('txtAccount').value = this.account;	
				if(this.content != null)
					el('txtContent').value = this.content;	
				return;
			}
		}
		this.feedback_loading();
		
		this.xmlhttp = create_request();
		this.xmlhttp.onreadystatechange = this.sendmail_callback;
		
		var url = sign_up_mgr.dir + "ajax/send_fb.ashx?email=" + this.account + "&content=" + escape(this.content);
		this.xmlhttp.open("GET", url, true);
		this.xmlhttp.send(null);
		this.sendmail_callback();
    },
    
    sendmail_callback : function() {
		if (fb_mgr.xmlhttp.readyState == 4) {
			if (fb_mgr.xmlhttp.status == 200) {
				var text = fb_mgr.xmlhttp.responseText;
				if (text == "1") {
					fb_mgr.feedback_complete();
				} else {
					fb_mgr.feedback_failure();
				}
			}
		}			
	},
	
	feedback_loading : function() {
		el('alert_message').style.display = 'none';
		el('feedback_loading').style.display = 'block';
		el('feedback').style.display = 'none';
		this.showLoading();
	},

	feedback_failure : function() {
		el('alert_message').style.display = 'none';
    	el('feedback_loading').style.display = 'none';
		el('feedback').style.display = 'block';
		el('send_error').style.display='block';
		this.innerAccount = this.account;
		this.innerContent = this.content;
		if(this.account != null)
			el('txtAccount').value = this.account;		
		if(this.content != null)
			el('txtContent').value = this.content;
		this.showDialog();
		if (window.navigator.userAgent.indexOf("Firefox")>=1){
			if(this.account != null)
				el('txtAccount').value = this.account;	
			if(this.content != null)
				el('txtContent').value = this.content;		
		}
	},

	feedback_complete : function() {
		el("send_over").style.display='block';
		el('feedback_loading').style.display = 'none';
		this.showComplete();
		el('feedback').style.display = 'block';
		el("send_over").style.display='none';
		el('send_error').style.display='none';
		el('alert_message').style.display = 'none';
		this.innerAccount = this.account;
		this.innerContent = this.content;
		el('txtAccount').value = this.account;
		if(this.content != null) {
			el('txtContent').value = "";
			this.content = null;
		}
		window.setTimeout(tb_remove, 2000);
	}
}