/************************************************************************************************************
Static folder tree
Copyright (C) October 2005  DTHMLGoodies.com, Alf Magne Kalleland

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Dhtmlgoodies.com., hereby disclaims all copyright interest in this script
written by Alf Magne Kalleland.

Alf Magne Kalleland, 2006
Owner of DHTMLgoodies.com

************************************************************************************************************/

/*
	Update log:
	December, 19th, 2005 - Version 1.1: Added support for several trees on a page(Alf Magne Kalleland)
	January,  25th, 2006 - Version 1.2: Added onclick event to text nodes.(Alf Magne Kalleland)
	February, 3rd 2006 - Dynamic load nodes by use of Ajax(Alf Magne Kalleland)
*/
/*
* Yury - remove some unneeded staff and make some modifications according our needs
*/

  var initExpandedNodes = '';	// Cookie - initially expanded nodes;
	var useAjaxToLoadNodesDynamically = true;
	var ajaxRequestFile = 'tabmenu.php';
	var contextMenuActive = false;	// Set to false if you don't want to be able to delete and add new nodes dynamically

	var ajaxObjectArray = new Array();
	var treeUlCounter = 0;
	var nodeId = 1;

	/*
	These cookie functions are downloaded from
	http://www.mach5.com/support/analyzer/manual/html/General/CookiesJavaScript.htm
	*/
	function Get_Cookie(name) {
	   var start = document.cookie.indexOf(name+"=");
	   var len = start+name.length+1;
	   if ((!start) && (name != document.cookie.substring(0,name.length))) return null;
	   if (start == -1) return null;
	   var end = document.cookie.indexOf(";",len);
	   if (end == -1) end = document.cookie.length;
	   return unescape(document.cookie.substring(len,end));
	}
	// This function has been slightly modified
	function Set_Cookie(name,value,expires,path,domain,secure) {
		expires = expires * 60*60*24*1000;
		var today = new Date();
		var expires_date = new Date( today.getTime() + (expires) );
	    var cookieString = name + "=" +escape(value) +
	       ( (expires) ? ";expires=" + expires_date.toGMTString() : "") +
	       ( (path) ? ";path=" + path : "") +
	       ( (domain) ? ";domain=" + domain : "") +
	       ( (secure) ? ";secure" : "");
	    document.cookie = cookieString;
	}

	function expandAll(treeId)
	{
		var menuItems = document.getElementById(treeId).getElementsByTagName('LI');
		for(var no=0;no<menuItems.length;no++){
			var subItems = menuItems[no].getElementsByTagName('UL');
			if(subItems.length>0 && subItems[0].style.display!='block'){
				showHideNode(false,menuItems[no].id.replace(/[^0-9]/g,''));
			}
		}
	}

	function collapseAll(treeId)
	{
		var menuItems = document.getElementById(treeId).getElementsByTagName('LI');
		for(var no=0;no<menuItems.length;no++){
			var subItems = menuItems[no].getElementsByTagName('UL');
			if(subItems.length>0 && subItems[0].style.display=='block'){
				showHideNode(false,menuItems[no].id.replace(/[^0-9]/g,''));
			}
		}
	}

	function getNodeDataFromServer(ajaxIndex,ulId,parentId)
	{
		document.getElementById(ulId).innerHTML = ajaxObjectArray[ajaxIndex].response;
		ajaxObjectArray[ajaxIndex] = false;
		parseSubItems(ulId,parentId);
	}


	function parseSubItems(ulId,parentId)
	{

		if(initExpandedNodes){
			var nodes = initExpandedNodes.split(',');
		}
		var branchObj = document.getElementById(ulId);
		var menuItems = branchObj.getElementsByTagName('LI');	// Get an array of all menu items
		for(var no=0;no<menuItems.length;no++){
			var imgs = menuItems[no].getElementsByTagName('IMG');
			//if(imgs.length>0)continue;
			nodeId++;
			var subItems = menuItems[no].getElementsByTagName('UL');
			var img = document.createElement('IMG');
			img.src = plusImage;
			img.onclick = showHideNode;
			if(subItems.length==0)img.style.visibility='hidden';else{
				subItems[0].id = 'tree_ul_' + treeUlCounter;
				treeUlCounter++;
			}
			var aTag = menuItems[no].getElementsByTagName('IMG')[0];

			menuItems[no].insertBefore(img,aTag);
			menuItems[no].id = 'dhtmlgoodies_treeNode' + nodeId;

			var tmpParentId = menuItems[no].getAttribute('parentId');
			if(!tmpParentId)tmpParentId = menuItems[no].tmpParentId;
			if(tmpParentId && nodes[tmpParentId])showHideNode(false,nodes[no]);
		}
	}

	function showHideNode(e,inputId)
	{
		if(inputId){
			if(!document.getElementById('dhtmlgoodies_treeNode'+inputId))return;
			thisNode = document.getElementById('dhtmlgoodies_treeNode'+inputId).getElementsByTagName('IMG')[0];
		}else {
			thisNode = this;
			if(this.tagName=='A')thisNode = this.parentNode.getElementsByTagName('IMG')[0];

		}
		if(thisNode.style.visibility=='hidden')return;
		var parentNode = thisNode.parentNode;
		inputId = parentNode.id.replace(/[^0-9]/g,'');
		if(thisNode.src.indexOf(plusName)>=0){
			thisNode.src = thisNode.src.replace(plusName,minusName);
			var ul = parentNode.getElementsByTagName('UL')[0];
			ul.style.display='block';
			if(!initExpandedNodes)initExpandedNodes = ',';
			if(initExpandedNodes.indexOf(',' + inputId + ',')<0) initExpandedNodes = initExpandedNodes + inputId + ',';

			if(useAjaxToLoadNodesDynamically){	// Using AJAX/XMLHTTP to get data from the server
				var firstLi = ul.getElementsByTagName('LI')[0];
				var parentId = firstLi.getAttribute('parentId');
				if(!parentId)parentId = firstLi.parentId;
				var parentType = firstLi.getAttribute('parentType');
				if(!parentType)parentType = firstLi.parentType;
				if(parentId){
					ajaxObjectArray[ajaxObjectArray.length] = new sack();
					var ajaxIndex = ajaxObjectArray.length-1;
					ajaxObjectArray[ajaxIndex].requestFile = ajaxRequestFile + '?parentId=' + parentId  + '&parentType=' + parentType;
					ajaxObjectArray[ajaxIndex].onCompletion = function() { getNodeDataFromServer(ajaxIndex,ul.id,parentId); };	// Specify function that will be executed after file has been found
					ajaxObjectArray[ajaxIndex].runAJAX();		// Execute AJAX function
				}
			}

		}else{
			thisNode.src = thisNode.src.replace(minusName,plusName);
			parentNode.getElementsByTagName('UL')[0].style.display='none';
			initExpandedNodes = initExpandedNodes.replace(',' + inputId,'');
		}
		Set_Cookie('dhtmlgoodies_expandedNodes',initExpandedNodes,500);

		return false;
	}

	var okToCreateSubNode = true;

	function initTree()
	{

		for(var treeCounter=0;treeCounter<idOfFolderTrees.length;treeCounter++){
			var dhtmlgoodies_tree = document.getElementById(idOfFolderTrees[treeCounter]);
			var menuItems = dhtmlgoodies_tree.getElementsByTagName('LI');	// Get an array of all menu items
			for(var no=0;no<menuItems.length;no++){
				nodeId++;
				var subItems = menuItems[no].getElementsByTagName('UL');
				var img = document.createElement('IMG');
				img.src = plusImage;
				img.onclick = showHideNode;
				if(subItems.length==0)img.style.visibility='hidden';else{
					subItems[0].id = 'tree_ul_' + treeUlCounter;
					treeUlCounter++;
				}
				var aTag = menuItems[no].getElementsByTagName('IMG')[0];
				menuItems[no].insertBefore(img,aTag);
				if(!menuItems[no].id)menuItems[no].id = 'dhtmlgoodies_treeNode' + nodeId;
			}

		}
		initExpandedNodes = Get_Cookie('dhtmlgoodies_expandedNodes');
		if(initExpandedNodes){
			var nodes = initExpandedNodes.split(',');
			for(var no=0;no<nodes.length;no++){
				if(nodes[no])showHideNode(false,nodes[no]);
			}
		}
	}

	window.onload = initTree;