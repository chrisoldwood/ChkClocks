ALTER TABLE [dbo].[Leagues] DROP CONSTRAINT FK_Leagues_Regions
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[Regions]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Regions]
GO

CREATE TABLE [dbo].[Regions] (
	[ID] [int] NOT NULL ,
	[Name] [varchar] (50) NOT NULL ,
	[Row] [int] IDENTITY (1, 1) NOT NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[Regions] WITH NOCHECK ADD 
	CONSTRAINT [PK_Regions] PRIMARY KEY  NONCLUSTERED 
	(
		[ID]
	)  ON [PRIMARY] 
GO

