<html>

<head>
<title>UK Leagues</title>
<!-- #INCLUDE VIRTUAL="/shared/metatags.asp" -->
</head>

<body>
<div align="center">

<!-- #INCLUDE VIRTUAL="/shared/mainmenu.asp" -->

<!-- Content table -->
<table width="600">
<tr><td>

<%
	Dim oRS

	' Create a recordset.
	Set oRS = Server.CreateObject("ADODB.RecordSet")

	oRS.ActiveConnection = Session("DB")
%>

<!-- Table of sports/games -->
<table width="100%">
	<tr>
		<td align="center">
			Browse Leagues by Sport/Game
		</td>
	</tr>
	<tr>
		<td align="center">
			<table><tr>
<%
				' Query for all games
				oRS.Open "SELECT ID, Name FROM Games ORDER BY Name"

				while not oRS.EOF
%>
				<td align="center"><a href="browsebygame.asp?id=<%= oRS.Fields("ID") %>"><%= oRS.Fields("Name") %></a></td>
<%
					oRS.MoveNext
				wend

				oRS.Close
%>
			</tr></table>
		</td>
	</tr>
</table>

<p></p>

<!-- Table of regions -->
<table width="100%">
	<tr>
		<td align="center">
			Browse Leagues by Region
		</td>
	</tr>
	<tr>
		<td align="center">
			<table><tr>
<%
				' Query for all regions
				oRS.Open "SELECT ID, Name FROM Regions ORDER BY Name"

				while not oRS.EOF
%>
				<td align="center"><a href="browsebyregion.asp?id=<%= oRS.Fields("ID") %>"><%= oRS.Fields("Name") %></a></td>
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
