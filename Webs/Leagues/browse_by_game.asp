<html>

<head>
<title>UK Leagues</title>
<!-- #INCLUDE VIRTUAL="/uk-leagues/shared/metatags.asp" -->
</head>

<%
	Dim nID

	' Extract game ID from URL.
	nID = CInt(Request.QueryString("id"))
%>

<body>
<div align="center">

<!-- #INCLUDE VIRTUAL="/uk-leagues/shared/main_menu.asp" -->

<!-- Content table -->
<table width="600">
<tr><td>

<%
	Dim oRS

	' Create a recordset.
	Set oRS = Server.CreateObject("ADODB.RecordSet")

	oRS.ActiveConnection = Session("DB")
%>

<%
	Dim strGame

	' Find game name from ID.
	oRS.Open "SELECT Name FROM Games WHERE ID=" & nID

	if not oRS.EOF then
		strGame = oRS.Fields("Name")
	end if

	oRS.Close
%>

<!-- Table of leagues -->
<table width="100%">
	<tr>
		<td align="center">
			<%= strGame %> Leagues
		</td>
	</tr>
	<tr>
		<td align="center">
			<table><tr>
<%
				' Query for all leagues for the given game ID.
				oRS.Open "SELECT ID, Name FROM Leagues WHERE GameID=" & nID &" ORDER BY Name"

				while not oRS.EOF
%>
				<td align="center"><a href=".asp?id=<%= oRS.Fields("ID") %>"><%= oRS.Fields("Name") %></a></td>
<%
					oRS.MoveNext
				wend

				oRS.Close
%>
			</tr></table>
		</td>
	</tr>
</table>

<!-- Content table -->
</td></tr>
</table>

</div>
</body>

</html>
