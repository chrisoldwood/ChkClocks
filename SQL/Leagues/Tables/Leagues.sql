if exists (select * from sysobjects where id = object_id(N'[dbo].[Leagues]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Leagues]
GO

CREATE TABLE [dbo].[Leagues] (
	[ID] [int] NOT NULL ,
	[GameID] [int] NOT NULL ,
	[RegionID] [int] NOT NULL ,
	[Name] [varchar] (100) NOT NULL ,
	[Row] [int] IDENTITY (1, 1) NOT NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[Leagues] WITH NOCHECK ADD 
	CONSTRAINT [PK_Leagues] PRIMARY KEY  NONCLUSTERED 
	(
		[ID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Leagues] ADD 
	CONSTRAINT [FK_Leagues_Games] FOREIGN KEY 
	(
		[GameID]
	) REFERENCES [dbo].[Games] (
		[ID]
	),
	CONSTRAINT [FK_Leagues_Regions] FOREIGN KEY 
	(
		[RegionID]
	) REFERENCES [dbo].[Regions] (
		[ID]
	)
GO

