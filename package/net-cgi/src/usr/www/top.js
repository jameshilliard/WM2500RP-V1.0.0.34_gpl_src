function languageShowlist()
{
	document.write('<option value=Auto>$auto_mark</option>');
	if (language_oriArray != "")
	{
		for (j=0; j<language_oriArray.length; j++)
		{
			document.write('<option value='+language_oriArray[j]+'>'+language_showArray[j]+'</option>');
		}
	}
	else
		document.write('<option value=English>'+language_oriArray[0]+'</option>');
}

function doDisable()
{
	form=document.forms[0];
	if( form.lang_avi.value == lang_select && !(lang_select == "Auto" && browser_lang != gui_region))
		location.reload();
	else
	{
		form.lang_avi.disabled=true;
        form.hidden_lang_avi.value=form.lang_avi.value;
		form.target="formframe";
        form.submit();
	}
}
function load_top_value()
{
    var upgrade_div = document.getElementById("update_info");
	if(upgrade_div != null)
	{
		if(wan_status==1 && config_status==9999 && updateFirmware==1)
			upgrade_div.style.display = "inline";
		else
			upgrade_div.style.display = "none";
	}
	
}
function firmwareUpgrade()
{
	goto_formframe('ver_lang_firm.htm');
}

function do_search()
{
	var key = document.forms[0].search.value.replace(/ /g,"%20");
	var winoptions = "width=960,height=800,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes";
	var url="http://kb.netgear.com/app/answers/list/kw/"+key;

	window.open(url,null,winoptions);
}
