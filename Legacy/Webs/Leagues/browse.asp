<html>

<head>
<title>UK Leagues</title>
<!-- #INCLUDE VIRTUAL="/uk-leagues/shared/metatags.asp" -->
</head>

<body>
<div align="center">

<!-- #INCLUDE VIRTUAL="/uk-leagues/shared/main_menu.asp" -->

<!-- Content table -->
<table width="600">
<tr><td>

<%
	Dim oRS
	Dim nCols

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
			<table>
<%
				' Query for all games
				oRS.Open "SELECT ID, Name FROM Games ORDER BY Name"

				' Number of columns per row
				nCols = 4

				while not oRS.EOF
%>
				<tr>
<%
					while (not oRS.EOF) And (nCols > 0)
%>
						<td align="center"><a href="browse_by_game.asp?id=<%= oRS.Fields("ID") %>"><%= oRS.Fields("Name") %></a></td>
<%
						oRS.MoveNext

						nCols = nCols - 1
					wend

					nCols = 4
%>
				</tr>
<%
				wend

				oRS.Close
%>			
			</table>
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
			<table>
<%
				' Query for all regions
				oRS.Open "SELECT ID, Name FROM Regions ORDER BY Name"

				' Number of columns per row
				nCols = 4

				while not oRS.EOF
%>
				<tr>
<%
					while (not oRS.EOF) And (nCols > 0)
%>
						<td align="center"><a href="browse_by_region.asp?id=<%= oRS.Fields("ID") %>"><%= oRS.Fields("Name") %></a></td>
<%
						oRS.MoveNext

						nCols = nCols - 1
					wend

					nCols = 4
%>
				</tr>
<%
				wend

				oRS.Close
%>			
			</table>
		</td>
	</tr>
</table>

<!-- Content table -->
</td></tr>
</table>

</div>
</body>

</html>
