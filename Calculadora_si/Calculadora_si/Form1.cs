namespace Calculadora_si
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void agregarnumero(string numero)
        {
            if (Resultado.Text == "0")
            {
                Resultado.Text = "";
            }
            Resultado.Text += numero;
        }
        private void button15_Click(object sender, EventArgs e)
        {

        }

        private void button18_Click(object sender, EventArgs e)
        {

        }

        private void button21_Click(object sender, EventArgs e)
        {

        }
    }
}
