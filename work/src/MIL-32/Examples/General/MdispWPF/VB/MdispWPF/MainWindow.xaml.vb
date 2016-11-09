Imports System
Imports System.ComponentModel
Imports System.Windows

Imports MILApplicationLibrary

Class MainWindow

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Event handler : Called when the window is loaded.
    ''' </summary>
    ''' <param name="sender">The object sending this event, in this case, the MainWindow.</param>
    ''' <param name="e">A <see cref="RoutedEventArgs"/> object providing state information and event data associated with this routed event.</param>
    Private Sub Window_Loaded(ByVal sender As Object, ByVal e As RoutedEventArgs) Handles MyBase.Loaded
        Try
            ' Instanciate a new MILApplication object.
            Dim application As New MILApplication()

            ' Allocate the MIL application.
            application.Allocate()

            ' Set the control used to host the display
            application.SetDisplayHost(_displayControlHost)

            ' Set the MILApplication object as the DataContext for this window objet.
            ' This will enable data bindings between the window and the MILApplication object.
            DataContext = application

            ' Mark the routed event as handled.
            e.Handled = True
        Catch ex As Exception
            MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error)
        End Try
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Event handler : Called when the Start Grab button is clicked.
    ''' </summary>
    ''' <param name="sender">The object sending this event, in this case, the Start Grab Button.</param>
    ''' <param name="e">A <see cref="RoutedEventArgs"/> object providing state information and event data associated with this routed event.</param>
    Private Sub StartGrab_Click(ByVal sender As Object, ByVal e As RoutedEventArgs)
        Try
            Dim milApplication As MILApplication = TryCast(DataContext, MILApplication)
            If milApplication IsNot Nothing Then
                ' Start grabbing live images.
                milApplication.StartGrab()

                ' Mark the routed event as handled.
                e.Handled = True
            End If
        Catch ex As Exception
            MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error)
        End Try
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Event handler : Called when the Stop Grab button is clicked.
    ''' </summary>
    ''' <param name="sender">The object sending this event, in this case, the Stop Grab Button.</param>
    ''' <param name="e">A <see cref="RoutedEventArgs"/> object providing state information and event data associated with this routed event.</param>
    Private Sub StopGrab_Click(ByVal sender As Object, ByVal e As RoutedEventArgs)
        Try
            Dim milApplication As MILApplication = TryCast(DataContext, MILApplication)
            If milApplication IsNot Nothing Then
                ' Stop grabbing live images.
                milApplication.StopGrab()

                ' Mark the routed event as handled.
                e.Handled = True
            End If
        Catch ex As Exception
            MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error)
        End Try
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Event handler : Called when the window is closing.
    ''' </summary>
    ''' <param name="sender">The object sending this event, in this case, the MainWindow.</param>
    ''' <param name="e">A <see cref="CancelEventArgs"/> object providing data about this cancelable event.</param>
    Private Sub Window_Closing(ByVal sender As Object, ByVal e As CancelEventArgs) Handles MyBase.Closing
        Try
            Dim milApplication As MILApplication = TryCast(DataContext, MILApplication)
            If milApplication IsNot Nothing Then
                ' Free the MILApplication to release unmananged resources.
                milApplication.Free()
            End If

            DataContext = Nothing
        Catch ex As Exception
            MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error)
        End Try
    End Sub
End Class
