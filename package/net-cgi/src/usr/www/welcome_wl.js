function sectype(wl_sectype)
{
    if(wl_sectype==2)
	wl_sectype="WEP";
	else if(wl_sectype==3)
	wl_sectype="WPA-PSK";
	else if(wl_sectype==4)
	wl_sectype="WPA2-PSK";
	else if(wl_sectype==5)
	wl_sectype="WPA/WPA2-PSK";
	else
	wl_sectype="NONE";
		
}
function check_welcome_ssid()
{
	var ssid = document.forms[0].wl_ssid.value;
	if(ssid == "")
	{
		alert("$ssid_null");
		return false;
	}
	if(ssid.toLowerCase() == "any")
	{
		alert(ssid + "$ssid_not_allowed");
		return false;
	}
	parent.wl0_ssid=document.forms[0].wl_ssid.value;
	location.href="ca_06_DoYouWantWirelessSecurity.htm";
}
function check_welcome_apssid()
{
    var ssid = document.forms[0].wl_ssid.value;
	if(ssid == "")
	{
		alert("$ssid_null");
		return false;
	}
	if(document.forms[0].same_ssid.checked==true)
	{
	    document.forms[0].wl_ssid.value=parent.wlan_ap_wifi;
		document.forms[0].wl_ssid.disabled=true;		
		parent.same_sec_type=1;
	}
	else
	{   var hidden_ssid=document.forms[0].wl_ssid.value;
	    parent.same_sec_type=0;
	}
	if(ssid.toLowerCase() == "any")
	{
		alert(ssid + "$ssid_not_allowed");
		return false;
	}
	parent.wl0_ssid=hidden_ssid;
	if(document.forms[0].same_ssid.checked==true)
	{
		if(wl_ap_sectype==3 || wl_ap_sectype==4||wl_ap_sectype==5)
        location.href="ca_15B_Summary_Page_WPA-PSK_PassPhrase.htm";
		else
		location.href="ca_15_Summary_Page.htm";
	}
	else
	location.href="ca_wireless_mode.htm";
}
function config_wireless_mode()
{  
   var cf=document.forms[0];
   cf.time_stamp_dni.value=parent.index_ts;
   if(cf.choice[0].checked)
   parent.choice=0;
   else
   parent.choice=1;
    if(dns_hijack==0)
    {
	   cf.action="/cgi-bin/setobject.cgi?/cgi-bin/ca_06_DoYouWantWirelessSecurity.htm";
    }
    else
    {
	   if(IA_from==0)
	   cf.action="/cgi-bin/setobject.cgi?/cgi-bin/setup_wizard.htm";
	   else
	   cf.action="/cgi-bin/setobject.cgi?/cgi-bin/wait_for_searching.htm";
	}
	cf.submit();
}
function check_want_sec()
{
	var cf=document.forms[0];
	cf.time_stamp_dni.value=parent.index_ts;
	if(cf.choice[1].checked)
		check_goto_sum();
	else if(cf.choice[0].checked)
		location.href="ca_07_SecurityOptions.htm";
	else
		return false;
}
function check_sec_options()
{
	var cf=document.forms[0];
	cf.time_stamp_dni.value=parent.index_ts;
	if(cf.choice[0].checked)
	{
		location.href="ca_08_WPA_PassPhrase.htm";
		parent.wl0_sec_mode="WPA1WPA2";
	}
	else if(cf.choice[2].checked)
	{
		location.href="ca_08_WPA_PassPhrase.htm";
		parent.wl0_sec_mode="psk";
	}
	else if(cf.choice[1].checked)
	{
		location.href="ca_08_WPA_PassPhrase.htm";
		parent.wl0_sec_mode="WPA2";
	}
	else if(cf.choice[3].checked)
	{
		location.href="ca_09_DoYouHave_WEP_Key.htm";
		parent.wl0_sec_mode = "wep";
	}
	else if(cf.choice[4].checked)
	{
		parent.wl0_sec_mode = "none";
		check_goto_sum()
	}
	else
		return false;
	return true;
}
function show_wireless_mode()
{
     if(parent.choice=="0")
     document.write(ca_wl_mode_145);
     else
     document.write(ca_wl_mode_300);
}
function check_tkip_aes()
{
	var cf=document.forms[0];
	cf.time_stamp_dni.value=parent.index_ts;
	if(cf.passphrase.value.length < 8)
    {
        alert(passphrase_short8);
        return false;
    }
    if(cf.passphrase.value.length > 63)
    {
        alert(passphrase_long63);
        cf.passphrase.value = "";
        return false;    
    }
	parent.wl0_pskkey = cf.passphrase.value;
    parent.wl0_wpaenc = 2; //tkip+aes
	check_goto_sum();
}
function goback()
{
	var cf = document.forms[0];
 	var  sec_mode=parent.wl0_sec_mode;
    if(sec_mode=="WPA1WPA2" || sec_mode=="psk" || sec_mode=="WPA2" )
		location.href="ca_08_WPA_PassPhrase.htm";
	else if(sec_mode=="wep")
		location.href="ca_09_DoYouHave_WEP_Key.htm";
	else
		location.href="ca_07_SecurityOptions.htm";
}
function check_wep_goto()
{
	var cf=document.forms[0];
	if(cf.choice[1].checked) 
		location.href="ca_10_EnterWEP_Key.htm";
	else if(cf.choice[0].checked) 
		location.href="ca_12_Enter_WEP_PassPhrase.htm";
	else
		return false;
}

function isHex(str) {
    var i;
    for(i = 0; i<str.length; i++) {
        var c = str.substring(i, i+1);
        if(("0" <= c && c <= "9") || ("a" <= c && c <= "f") || ("A" <= c && c <= "F")) {
            continue;
        }
        return false;
    }
    return true;
}

function check_wep_key(){
	var msg_key64_type = "Invalid Key.\n\nKeys length should be 10 Hex (0~9 and A~F) chars. " + "\nDefault key is required, other keys are optional.";
    var msg_key128_type = "Invalid Key.\n\nKeys length should be 26 Hex (0~9 and A~F) chars. " + "\nDefault key is required, other keys are optional.";
    var msg_key152_type = "Invalid Key.\n\nKeys length should be 32 Hex (0~9 and A~F) chars. " + "\nDefault key is required, other keys are optional.";
    var cf = document.forms[0];
	cf.time_stamp_dni.value=parent.index_ts;
    var msg = ""; var blankFlag = false; var sizeOK = false; var i,j;
    var len = (cf.KEY1.value.length<=10)?10:((cf.KEY1.value.length<=26)?26:32);
	  
        msg = "";
       
        sizeOK = (isHex(cf.KEY1.value)) && (cf.KEY1.value.length == len);
        if (!sizeOK)
        {
            msg = (len == 10)?msg_key64_type:msg_key128_type;
            alert(msg);
            cf.KEY1.focus(); 
			location.href="ca_11_Error_WEP_Key.htm";               
            return false;
        }	
	if(len==10){
		parent.wl0_keylength=40;
	}else{
		parent.wl0_keylength=104;
	}
	parent.wl0_sec_mode = "wep";
	parent.wl0_key1=cf.KEY1.value;
	check_goto_sum();
}

function check_wep_error()
{
	var cf=document.forms[0];
	if(cf.choice[0].checked)
		location.href="ca_10_EnterWEP_Key.htm";		
	else if(cf.choice[1].checked)
		location.href="ca_12_Enter_WEP_PassPhrase.htm";
	else
		return false;
	return true;
}

function check_wep_creatkey()
{
	var cf=document.forms[0];
	if (calcPassphrase_welcome(cf))
	{
		if (cf.wepenc.selectedIndex == 0)
    		parent.wl0_keylength = 40;			//64-bits
    	else
    		parent.wl0_keylength = 104;			//128-bits  
		
		parent.wl0_txkey=1;
		parent.wl0_key1 = cf.KEY1.value;
		parent.wl0_pskkey=cf.passphraseStr.value;
	}
	else
		return false;
	location.href="ca_12A_WEP_PassPhrase_Page.htm";
}
function calcPassphrase_welcome(F)
{
    if(F.passphraseStr.value.length == 0){
        alert(wep_null);
		return false;
   }
    switch(F.wepenc.selectedIndex)
    {
        case 0:
            PassPhrase40_welcome(F);
            break;
        case 1:
            PassPhrase104_welcome(F);
            break;
        default:
            break;
    }
	return true;
}
function PassPhrase40_welcome(F)
{
    var seed = 0;
    var pseed = new Array(0, 0, 0, 0);
    var pkey = new Array(4);
    var asciiObj = new Array(4);
    Length = F.passphraseStr.value.length;
    if(Length != 0)
    {
        for (i=0; i<Length; i++ )
            pseed[i%4] ^= F.passphraseStr.value.charCodeAt(i);
        seed = pseed[0];
        seed += pseed[1] << 8;
        seed += pseed[2] << 16;
        seed += pseed[3] << 24;
    }
    F.KEY1.value = F.KEY2.value = "";
    F.KEY3.value = F.KEY4.value = "";
    pkey[0] = F.KEY1;
    pkey[1] = F.KEY2;
    pkey[2] = F.KEY3;
    pkey[3] = F.KEY4;
    for(j=0; j<4; j++)
    {
        for (i=0; i<5 ;i++ )
        {
            seed = (214013 * seed) & 0xffffffff;
            if(seed & 0x80000000)
                seed = (seed & 0x7fffffff) + 0x80000000 + 0x269ec3;
            else
                seed = (seed & 0x7fffffff) + 0x269ec3;
            temp = ((seed >> 16) & 0xff);
            if(temp < 0x10)
                pkey[j].value += "0" + temp.toString(16).toUpperCase();
            else
                pkey[j].value += temp.toString(16).toUpperCase();
        }
    }
	return true;
}
function PassPhrase104_welcome(F)
{
    var pseed2 = "";
    Length2 = F.passphraseStr.value.length;
    var asciiObj = "";
    for(p=0; p<64; p++)
    {
        tempCount = p % Length2;
        pseed2 += F.passphraseStr.value.substring(tempCount, tempCount+1);
    }
    md5Str = calcMD5(pseed2);
    F.KEY1.value = md5Str.substring(0, 26).toUpperCase();
	return true;
    F.KEY2.value = F.KEY1.value;
    F.KEY3.value = F.KEY1.value;
    F.KEY4.value = F.KEY1.value;
    F.wep_key_no[0].checked = true;
    F.KEY2.disabled = F.KEY3.disabled = F.KEY4.disabled = true;
}

function check_goto_sum()
{
	var cf=document.forms[0]; 
	cf.time_stamp_dni.value=parent.index_ts;
	var next_page;
	
		parent.temp_password="password";
		var  sec_mode=parent.wl0_sec_mode;
		if(sec_mode=="WPA1WPA2" || sec_mode=="psk" || sec_mode=="WPA2" ){
			cf.action="/cgi-bin/setobject.cgi?/cgi-bin/ca_15B_Summary_Page_WPA-PSK_PassPhrase.htm";
		}else if(sec_mode=="wep" && parent.wl0_pskkey==""){
			cf.action="/cgi-bin/setobject.cgi?/cgi-bin/ca_15A_Summary_Page_WEP_PassPhrase.htm";
		}else{
			cf.action="/cgi-bin/setobject.cgi?/cgi-bin/ca_15_Summary_Page.htm";
		}
	check_summary_tmp();
}

function show_sec_mode(type){
	if(type=="WPA1WPA2"){
		document.write("WPA-PSK(TKIP) + WPA2-PSK(AES)");
	}else if(type=="WPA2"){
		document.write("WPA2-PSK(AES)");
	}else if(type=="psk"){
		document.write("WPA-PSK(TKIP)");
	}else if(type=="wep"){
		document.write("WEP");
	}else{
		document.write("None");
	}
}
function show_region_name()
{
if(parent.wl_country == ZA)
  document.write(coun_af);
if(parent.wl_country == CN)
  document.write(coun_as);
if(parent.wl_country == AU)
  document.write(coun_au);
if(parent.wl_country == CA)
  document.write(coun_ca);
if(parent.wl_country == FR)
  document.write(coun_eu);
if(parent.wl_country == IL)
  document.write(coun_is);
if(parent.wl_country == JP)
  document.write(coun_jp);
if(parent.wl_country == KR)
  document.write(coun_ko);
if(parent.wl_country == MX)
  document.write(coun_mx);
if(parent.wl_country == BR)
  document.write(coun_sa);
if(parent.wl_country == US)
  document.write(coun_us);
}

function check_summary()
{
	var cf = document.forms[0];
	cf.time_stamp_dni.value=parent.index_ts;
	if(ap_client==1)
	{
	   /*
	    cf.sec_wpaphrase_len.value=0;
		cf.basic_type.value="1";   
		cf.country.value=country_code;
		cf.ssid.value=wlan_ap_wifi;
		cf.channel.value=wlan_get_channel;
	    cf.wireless_mode.value=wlan_get_mode;
		var type=wl_ap_sectype;
		if(wl_ap_sectype==5)
		{
			cf.security_mode.value=5;
			cf.wpa1wpa2_shared_key.value=ap_get_wpas;
			cf.sec_wpaphrase_len.value=get_sec_wpaphrase_len;
		}else if(wl_ap_sectype==3)
		{
			cf.security_mode.value=3;
			cf.wpa1_shared_key.value=ap_get_wpa1;
			cf.sec_wpaphrase_len.value=get_sec_wpaphrase_len;		
		}
			else if(wl_ap_sectype==4)
		{
			cf.security_mode.value=4;
			cf.wpa2_shared_key.value=ap_get_wpa2;
			cf.sec_wpaphrase_len.value=get_sec_wpaphrase_len;		
		}
		else if(wl_ap_sectype==2)
		{
			cf.security_mode.value=2;
			cf.wep_encryption.value=wl_get_ap_keylength; 	
			cf.wep_key1.value=ap_wl_key1;
		}
		else
			cf.security_mode.value=1;
		*/
		cf.dns_hijack.value=0;
	}
	else
	{
	      cf.sec_wpaphrase_len.value=0;
		 cf.basic_type.value="1";   

		/*
		if(parent.welcome_wan_type == 6)
		{
			cf.basic_type.value="1";
		}
		else{
			cf.basic_type.value="0";
		}*/
		cf.country.value=country_code;
		cf.ssid.value=parent.wl0_ssid;
		cf.channel.value=parent.wl0_channel;
	        cf.wireless_mode.value=parent.wirelessMode;
		var type=parent.wl0_sec_mode;
		if(type=="WPA1WPA2")
		{
			cf.security_mode.value=5;
			cf.wpa1wpa2_shared_key.value=parent.wl0_pskkey;
			cf.sec_wpaphrase_len.value=parent.wl0_pskkey.length;
		}else if(type=="psk")
		{
			cf.security_mode.value=3;
			cf.wpa1_shared_key.value=parent.wl0_pskkey;
			cf.sec_wpaphrase_len.value=parent.wl0_pskkey.length;		
		}
			else if(type=="WPA2")
		{
			cf.security_mode.value=4;
			cf.wpa2_shared_key.value=parent.wl0_pskkey;
			cf.sec_wpaphrase_len.value=parent.wl0_pskkey.length;		
		}
		else if(type=="wep")
		{
			cf.security_mode.value=2;
			if(parseInt(parent.wl0_keylength)==40)
				cf.wep_encryption.value=5; 	
			else if(parseInt(parent.wl0_keylength)==104)	
				cf.wep_encryption.value=13; 	
			cf.wep_key1.value=parent.wl0_key1;
		}
		else
			cf.security_mode.value=1;
			
		cf.dns_hijack.value=0;
	}
	if(dns_hijack==0)
	{
	    cf.action="/cgi-bin/setobject.cgi?/cgi-bin/info.htm"
	}
	cf.submit();
}
function check_summary_tmp()
{
    var cf = document.forms[0];
    cf.sec_wpaphrase_len.value=0;
     cf.basic_type.value="1";

    /*
    if(parent.welcome_wan_type == 6)
    {
        cf.basic_type.value="1";
    }
    else{
        cf.basic_type.value="0";
    }*/
    cf.country.value=5;
    cf.ssid.value=parent.wl0_ssid;
    cf.channel.value=parent.wl0_channel;
    var type=parent.wl0_sec_mode;
    if(type=="WPA1WPA2")
    {
        cf.security_mode.value=5;
        cf.wpa1wpa2_shared_key.value=parent.wl0_pskkey;
        cf.sec_wpaphrase_len.value=parent.wl0_pskkey.length;
    }else if(type=="psk")
    {
        cf.security_mode.value=3;
        cf.wpa1_shared_key.value=parent.wl0_pskkey;
        cf.sec_wpaphrase_len.value=parent.wl0_pskkey.length;
    }
        else if(type=="WPA2")
    {
        cf.security_mode.value=4;
        cf.wpa2_shared_key.value=parent.wl0_pskkey;
        cf.sec_wpaphrase_len.value=parent.wl0_pskkey.length;
    }
    else if(type=="wep")
    {
        cf.security_mode.value=2;
        if(parseInt(parent.wl0_keylength)==40)
            cf.wep_encryption.value=5;
        else if(parseInt(parent.wl0_keylength)==104)
            cf.wep_encryption.value=13;
        cf.wep_key1.value=parent.wl0_key1;
    }
    else
        cf.security_mode.value=1;
    cf.submit();
}
