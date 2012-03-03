#!/bin/haserl
Content-type: application/octet-stream;
Content-Length: <% echo $((`du "${FORM_shared}" | cut -f1`*1024)) %>;
Content-Disposition: attachment; filename=<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
        FILENAME=${FORM_shared}
        echo \"$(basename "${FILENAME}")\";
        echo ""
	dd if="${FILENAME}" bs=2048
fi
%>
