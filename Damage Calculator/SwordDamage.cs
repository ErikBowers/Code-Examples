using System.Diagnostics;
//Project from Head First C# by Andrew Stellman and Jennifer Greene (Published by O'Reilly)

namespace BasicPractice
{
    class SwordDamage
    {
        private const int BASE_DAMAGE = 3;
        private const int FLAME_DAMAGE = 2;

        /// <summary>
        /// SwordDamage constructor
        /// Calculates sword damage based on default Magic and Flaming values and a starting 3d6 roll.
        /// </summary>
        /// <param name="roll">Starting 3d6 roll</param>
        public SwordDamage(int startingRoll)
        {
            roll = startingRoll;
            CalculateDamage();
        }

        private int roll;
        public int Roll
        {
            get { return roll; }
            set 
                { 
                    roll = value;
                    CalculateDamage();         
                }
        }//end property Roll

        private bool flaming;
        /// <summary>
        /// True if sword is flaming, false otherwise.
        /// </summary>
        public bool Flaming
        {
            get { return flaming; }
            set
            {
                flaming = value;
                CalculateDamage();
            }
        }//end property Flaming

        private bool magic;
        /// <summary>
        /// True if sword is magic, false otherwise
        /// </summary>
        public bool Magic
        {
            get { return magic; }
            set 
            { 
                magic = value;
                CalculateDamage();
            }
        }//end property Magic

        public int Damage { get; private set; }

        private void CalculateDamage()
        {
            decimal magicMultiplier = 1M;
            if (Magic) magicMultiplier = 1.75M;
            Damage = BASE_DAMAGE;
            Damage = (int)(Roll * magicMultiplier) + BASE_DAMAGE;
            if (Flaming) Damage += FLAME_DAMAGE;
            Debug.WriteLine($"CalculatedDamage finished: {Damage} (roll: {Roll})");
        }//end CalculateDamage()
        
    }//end class SwordDamage
}
