Imports Microsoft.VisualBasic
Imports System
Namespace MDispWindow
    Partial Public Class Form1
        ''' <summary>
        ''' Required designer variable.
        ''' </summary>
        Private components As System.ComponentModel.IContainer = Nothing

        ''' <summary>
        ''' Clean up any resources being used.
        ''' </summary>
        ''' <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        Protected Overrides Sub Dispose(ByVal disposing As Boolean)
            If disposing AndAlso (components IsNot Nothing) Then
                components.Dispose()
            End If
            MyBase.Dispose(disposing)
        End Sub

        #Region "Windows Form Designer generated code"

        ''' <summary>
        ''' Required method for Designer support - do not modify
        ''' the contents of this method with the code editor.
        ''' </summary>
        Private Sub InitializeComponent()
            Dim resources As New System.ComponentModel.ComponentResourceManager(GetType(Form1))
            Me.MainMenu = New System.Windows.Forms.MenuStrip()
            Me.startMenuItem = New System.Windows.Forms.ToolStripMenuItem()
            Me.MainMenu.SuspendLayout()
            Me.SuspendLayout()
            ' 
            ' MainMenu
            ' 
            Me.MainMenu.Items.AddRange(New System.Windows.Forms.ToolStripItem() { Me.startMenuItem})
            Me.MainMenu.Location = New System.Drawing.Point(0, 0)
            Me.MainMenu.Name = "MainMenu"
            Me.MainMenu.Size = New System.Drawing.Size(855, 24)
            Me.MainMenu.TabIndex = 0
            Me.MainMenu.Text = "menuStrip1"
            ' 
            ' startMenuItem
            ' 
            Me.startMenuItem.Name = "startMenuItem"
            Me.startMenuItem.Size = New System.Drawing.Size(43, 20)
            Me.startMenuItem.Text = "&Start"
'            Me.startMenuItem.Click += New System.EventHandler(Me.startMenuItem_Click);
            ' 
            ' Form1
            ' 
            Me.AutoScaleDimensions = New System.Drawing.SizeF(6F, 13F)
            Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
            Me.BackColor = System.Drawing.Color.FromArgb((CInt(Fix((CByte(160))))), (CInt(Fix((CByte(160))))), (CInt(Fix((CByte(164))))))
            Me.ClientSize = New System.Drawing.Size(855, 620)
            Me.Controls.Add(Me.MainMenu)
            Me.Icon = (CType(resources.GetObject("$this.Icon"), System.Drawing.Icon))
            Me.MainMenuStrip = Me.MainMenu
            Me.Name = "Form1"
            Me.Text = "MIL Application"
            Me.MainMenu.ResumeLayout(False)
            Me.MainMenu.PerformLayout()
            Me.ResumeLayout(False)
            Me.PerformLayout()

        End Sub

        #End Region

        Private MainMenu As System.Windows.Forms.MenuStrip
        Private WithEvents startMenuItem As System.Windows.Forms.ToolStripMenuItem
    End Class
End Namespace

