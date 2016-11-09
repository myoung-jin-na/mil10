using System;
using System.ComponentModel;
using System.Windows;

using MILApplicationLibrary;

namespace MdispWPF
{
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Creates a new instance of the MainWindow object,
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Event handler : Called when the window is loaded.
        /// </summary>
        /// <param name="sender">The object sending this event, in this case, the MainWindow.</param>
        /// <param name="e">A <see cref="RoutedEventArgs"/> object providing state information and event data associated with this routed event.</param>
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                // Instanciate a new MILApplication object.
                MILApplication application = new MILApplication();

                // Allocate the MIL application.
                application.Allocate();

                // Set the control used to host the display
                application.SetDisplayHost(_displayControlHost);

                // Set the MILApplication object as the DataContext for this window objet.
                // This will enable data bindings between the window and the MILApplication object.
                DataContext = application;

                // Mark the routed event as handled.
                e.Handled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Event handler : Called when the Start Grab button is clicked.
        /// </summary>
        /// <param name="sender">The object sending this event, in this case, the Start Grab Button.</param>
        /// <param name="e">A <see cref="RoutedEventArgs"/> object providing state information and event data associated with this routed event.</param>
        private void StartGrab_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                MILApplication milApplication = DataContext as MILApplication;
                if (milApplication != null)
                {
                    // Start grabbing live images.
                    milApplication.StartGrab();

                    // Mark the routed event as handled.
                    e.Handled = true;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Event handler : Called when the Stop Grab button is clicked.
        /// </summary>
        /// <param name="sender">The object sending this event, in this case, the Stop Grab Button.</param>
        /// <param name="e">A <see cref="RoutedEventArgs"/> object providing state information and event data associated with this routed event.</param>
        private void StopGrab_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                MILApplication milApplication = DataContext as MILApplication;
                if (milApplication != null)
                {
                    // Stop grabbing live images.
                    milApplication.StopGrab();

                    // Mark the routed event as handled.
                    e.Handled = true;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Event handler : Called when the window is closing.
        /// </summary>
        /// <param name="sender">The object sending this event, in this case, the MainWindow.</param>
        /// <param name="e">A <see cref="CancelEventArgs"/> object providing data about this cancelable event.</param>
        private void Window_Closing(object sender, CancelEventArgs e)
        {
            try
            {
                MILApplication milApplication = DataContext as MILApplication;
                if (milApplication != null)
                {
                    // Free the MILApplication to release unmananged resources.
                    milApplication.Free();
                }

                DataContext = null;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
