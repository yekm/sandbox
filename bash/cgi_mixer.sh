#!/bin/bash

source config.sh
v=$(amixer -c $mixer_id get Master)
pvalue=$(echo "$v" | perl -ne '/(\d?\d)%/ && print $1')

cat << EOF
Content-type: text/html
Expires: Thu, 29 Oct 1998 17:04:19 GMT\n

EOF

case "$QUERY_STRING" in
up)
    amixer -c $mixer_id set Master 1dB+
    ;;
down)
    amixer -c $mixer_id set Master 1dB-
    ;;
set*)
    amixer -c $mixer_id set Master ${QUERY_STRING##*=}%
    ;;
poweroff)
    sudo /sbin/poweroff 2>&1 && echo success
    echo poweroff
    ;;
*)
cat <<EOF
<html>
<head>
<style type="text/css">
.button
{
    background-color: #eef;
    width: 200px;
    height: 70px;
    text-align: center;
}
pre
{
    font-family:courier, "courier new", monospace;
    font-size:16pt;
    color:#000;
    background-color:#fff;
}
</style>
<script type="text/javascript">

var info_div;
var requp;
var reqdown;

function answerup()
{
    if (requp.readyState == 4 && requp.status == 200) {
        resp = requp.responseText;
        if (resp != '')
        {
            info_div.innerHTML = resp;
            percent_pattern = new RegExp("\\\\d?\\\\d%", "gim");
            m_progress.value = percent_pattern.exec(resp)[0].substr(0,2);
        }
        requp.open('GET', '$DOCUMENT_URI?up', true);
    }
}

function answerdown()
{
    if (reqdown.readyState == 4 && reqdown.status == 200) {
        resp = reqdown.responseText;
        if (resp != '')
        {
            info_div.innerHTML = resp;
            percent_pattern = new RegExp("\\\\d?\\\\d%", "gim");
            m_progress.value = percent_pattern.exec(resp)[0].substr(0,2);
        }
        reqdown.open('GET', '$DOCUMENT_URI?down', true);
    }
}

function answerset()
{
    if (reqset.readyState == 4 && reqset.status == 200) {
        resp = reqset.responseText;
        if (resp != '')
        {
            info_div.innerHTML = resp;
            percent_pattern = new RegExp("\\\\d?\\\\d%", "gim");
            m_progress.value = percent_pattern.exec(resp)[0].substr(0,2);
        }
    }
}

function answerpower()
{
    if (reqpower.readyState == 4 && reqpower.status == 200) {
        resp = reqpower.responseText;
        if (resp != '')
        {
            info_div.innerHTML = resp;
        }
        else
        {
            info_div.innerHTML = 'empty response';
        }
    }
}

function prclick(event)
{
    p = event.clientX/m_progress.offsetWidth*100;
    p = p.toFixed();
//    info_div.innerHTML = p;
    reqset.open('GET', '$DOCUMENT_URI?set='+p, true);
    reqset.send(null);
}

function onload() {
    info_div = document.getElementById('info');
    m_progress = document.getElementById('pr');
    requp = new XMLHttpRequest();
    reqdown = new XMLHttpRequest();
    reqset = new XMLHttpRequest();
    reqpower = new XMLHttpRequest();
    requp.onreadystatechange = answerup;
    reqdown.onreadystatechange = answerdown;
    reqset.onreadystatechange = answerset;
    reqpower.onreadystatechange = answerpower;
    requp.open('GET', '$DOCUMENT_URI?up', true);
    reqdown.open('GET', '$DOCUMENT_URI?down', true);
    reqpower.open('GET', '$DOCUMENT_URI?poweroff', true);
}

window.addEventListener('load', onload, false)

</script>
</head>
<body>
<pre>
<div id=info>
$v
</div>
</pre>
<br>
<table>
<tr>
    <td class=button onclick='reqdown.send(null);'>volume down</td>
    <td class=button onclick='requp.send(null);'>volume up</td>
</tr>
<tr>
    <td colspan=2><progress style="width:100%; height:20px;" id=pr value="$pvalue" max="100" onclick="prclick(event)"></td>
</tr>
<tr>
    <td colspan=2 class=button onclick='reqpower.send(null);'>poweroff</td>
</tr>
</table>
<br>
<!--
<pre>
(set)
</pre>
-->
</body></html>

EOF

;;
esac

