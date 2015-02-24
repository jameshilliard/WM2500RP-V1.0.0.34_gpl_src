//class: region words length

function class_region(btn_double, btn_triple, sub_double, sub_triple)
{
	var _this = this;	//save this to a private class variable to avoid some error may caused by event

	_this.adv_btn_double = btn_double;
	_this.adv_btn_triple = btn_triple;

	_this.sub_double = sub_double;
	_this.sub_triple = sub_triple;

	if(arguments.length == 6)
	{
		_this.white_double = arguments[4];
		_this.white_triple = arguments[5];
	}
	else
	{
		_this.white_double = _this.adv_btn_double;
		_this.white_triple = _this.adv_btn_triple;
	}
}

function initClass(region)
{
	var ret_class;
	switch(region)
	{
		//class_region(menu_double, menu_triple, sub_menu_double, sub_menu_triple, white_btns_double, white_btns_triple)
		case "French":
			ret_class = new class_region(23, 40, 22, 40);
			break;
		case "Italian":
			ret_class = new class_region(22, 40, 20, 35);
			break;
		case "Russian":
			ret_class = new class_region(22, 40, 21, 33);
			break;
		case "Spanish":
			ret_class = new class_region(20, 40, 21, 40);
			break;
		case "Japanese":
			ret_class = new class_region(11, 40, 11, 36);
			break;
		case "German":
			ret_class = new class_region(22, 40, 17, 36);
			break;
		default:
			ret_class = new class_region(20, 40, 22, 35);
			break;
	}

	return ret_class;
}
