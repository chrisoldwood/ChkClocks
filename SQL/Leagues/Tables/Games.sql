ALTER TABLE [dbo].[Leagues] DROP CONSTRAINT FK_Leagues_Games
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[Games]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Games]
GO

CREATE TABLE [dbo].[Games] (
	[ID] [int] NOT NULL ,
	[Name] [varchar] (50) NOT NULL ,
	[Row] [int] IDENTITY (1, 1) NOT NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[Games] WITH NOCHECK ADD 
	CONSTRAINT [PK_Games] PRIMARY KEY  NONCLUSTERED 
	(
		[ID]
	)  ON [PRIMARY] 
GO

