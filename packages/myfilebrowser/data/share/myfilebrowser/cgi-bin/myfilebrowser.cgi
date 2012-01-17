#!/bin/haserl --upload-limit=200000 --upload-dir=/usb/sda1
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">
<head>
<style>
a { cursor: pointer }
img { vertical-align: text-top }
</style>
<%

CURDIR=${FORM_folder}

list_dir(){
        DIR=${CURDIR:-/}
        echo "<b>Current directory: $DIR</b><font color=blue>"
        echo "&nbsp;&nbsp;&nbsp;<a onClick=\"setValue('$(echo /$CURDIR | sed "s,/[^/]*$,," | sed "s,^/,,")');folderlist.submit();\"><b>Move Up Directory</b></a>"
	echo "&nbsp;&nbsp;&nbsp;<a onClick=\"setValue('$CURDIR');folderlist.submit();\"><b>Refresh Current Directory</b></a>"
	echo "</font>"
	echo -n "<pre>"

        if [ "$DIR" != "/" ]
        then IMG="trash.gif"
        else IMG="empty.gif"
        fi

        DIR0=$CURDIR
        DIR=/usb${DIR0}

	echo "$(ls -lLh "${DIR}")" | while read LINE
	do
                #FILENAME=$(echo "$LINE" | cut -c 57-)
                FILENAME=${LINE:55}
		#FILE_PROPERTIES=$(echo "$LINE" | cut -c -55)
                FILE_PROPERTIES=${LINE:0:55}
                #ISDIR=$(echo "$LINE" | cut -c 1)
                ISDIR=${LINE:0:1}
                if [ "$ISDIR" = "d" ]
                then
			echo -n "<img style=\"cursor:pointer\" src=\"/img/$IMG\" onClick=\"if (ConfirmDeleteDir('${DIR0}/${FILENAME}')) {del.deldir.value='${DIR}/${FILENAME}';del.submit();}\"> <a onClick=\"setValue('${DIR0}/${FILENAME}');folderlist.submit();\">"
                else
                	echo -n "<img style=\"cursor:pointer\" src=\"/img/$IMG\" onClick=\"if (ConfirmDeleteFile('${DIR0}/${FILENAME}')) {del.delfilename.value='${DIR}/${FILENAME}';del.submit();}\"> <a onClick=\"if (ConfirmDownload('${DIR}/${FILENAME}')) {setValue('${DIR}/${FILENAME}');downloadfile.submit();}\">"
                fi
		echo -n "${FILE_PROPERTIES}"
		echo "<font color=blue>${FILENAME}</font></a>"
	done
        echo "</pre>"
}

current_folder(){
        DIR=${CURDIR:-/}
        echo "<b>Current directory: $DIR</b>"
}

space(){
        DIR=${CURDIR:-/}
        if [ "$DIR" != "/" ]
        then
                echo "<pre>"
                echo "$(df -h "/usb$DIR")" | while read LINE
                do
                        echo "$LINE"
                done
                echo "</pre>"
        fi
}

%>
<script language="JavaScript">
<!-- //

function setValue(value){
	folderlist.folder.value = value;
        downloadfile.shared.value = value;
	actions.folder.value = value;
        return true;
}

function ConfirmDeleteDir(filename){
        return confirm ('WARNING!!!\n\nAll files and subfolders will be DELETED if you confirm!\n\nDelete folder ' + filename + ' ?');
}

function ConfirmDeleteFile(filename){
        return confirm ('Delete file ' + filename + ' ?');
}

function ConfirmDownload(filename){
        return confirm ('Download file ' + filename + ' ?');
}

function toggle(divname){
	var ele = document.getElementById(divname);
	if(ele.style.display == "block") {
    		ele.style.display = "none";
  	}
	else {
		ele.style.display = "block";
	}
}

function togglecreatedir() {
        toggle("divcreatedir");
        toggle("divactions");
}

function toggleuploadfile() {
        toggle("divuploadfile");
        toggle("divactions");
}

// -->
</script>
</head>
<body style="cursor:text">

<%
ACTION=''
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=${FORM_action}
fi

if [ "${FORM_delfilename}" != "" ]
then
        rm "${FORM_delfilename}"
fi

if [ "${FORM_deldir}" != "" ]
then
        rm -r "${FORM_deldir}"
fi

if [ "$ACTION" = "Upload" ]
then
        if [ -n "$FORM_uploadfile_name" ] ; then
                dd if="$FORM_uploadfile" of="/usb${CURDIR}/${FORM_uploadfile_name}"
        fi
fi

if [ "$ACTION" = "Create" ]
then
        if [ "$FORM_createdir" != "" ] ; then
                mkdir "/usb$CURDIR/$FORM_createdir"
        fi
fi

%>

<form id=folderlist name=folderlist action="<%= /cgi-bin/myfilebrowser-entry.cgi %>" method="POST">
<% list_dir %>
<input type=hidden name=folder value="<%= ${CURDIR:-/} %>">
</form>

<form id=downloadfile name=downloadfile action="<%= /cgi-bin/mydownloadfile.cgi %>" method="POST">
<input type=hidden name=shared value="<%= ${CURDIR:-/} %>">
</form>

<form id=del name=del action="<%= /cgi-bin/myfilebrowser-entry.cgi %>" method="POST">
<input type=hidden name=deldir value="">
<input type=hidden name=delfilename value="">
<input type=hidden name=folder value="<%= ${CURDIR:-/} %>">
</form>

<form id=actions name=actions action="<%= /cgi-bin/myfilebrowser-entry.cgi %>" method="POST" enctype="multipart/form-data">

<%
DIR=${CURDIR:-/}
if [ "$DIR" != "/" ]
then
%>
<div id="divactions" style="display: block">
<input type=button name="btnuploadfile" style="cursor:pointer" onClick="toggleuploadfile();" value="Upload File">
<input type=button name="btncreatedir"  style="cursor:pointer" onClick="togglecreatedir();createdir.focus();" value="Create Directory">
</div>

<div id="divcreatedir" style="display: none">
Create Directory: <input type=text name=createdir>
<input type=hidden name=folder style="cursor:pointer" value="<%= ${CURDIR:-/} %>">
<input type=submit name="action" style="cursor:pointer" value="Create" onClick="if (createdir.value=='') {alert('Please enter a directory name'); return false;}">
<input type=button name="btncancel" style="cursor:pointer" value="Cancel" onClick="togglecreatedir();">
</div>

<div id="divuploadfile" style="display: none">
Upload File: <input type=file style="cursor:pointer" value="Upload File" name=uploadfile>
<input type=hidden name=folder value="<%= ${CURDIR:-/} %>">
<input type=submit name="action" style="cursor:pointer" value="Upload" onClick="if (uploadfile.value=='') {alert('Please choose a file'); return false;}">
<input type=button name="btncancel" style="cursor:pointer" value="Cancel" onClick="toggleuploadfile();">
</div>

<%
space
fi
%>
<input type=hidden name=folder value="<%= ${CURDIR:-/} %>">
</form>

</body>
</html>
