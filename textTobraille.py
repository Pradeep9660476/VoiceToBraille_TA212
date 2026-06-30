# Text to Braille Converter - Python Version
# Format: [dot1,dot2,dot3,dot4,dot5,dot6] where 1=raised, 0=flat
import numpy as np

class BrailleConverter:
    def __init__(self):
        self.braille_map = self.initialize_braille_map()
    
    def initialize_braille_map(self):
        """Initialize the braille pattern lookup table"""
        patterns = {
            # A-Z mapping
            'a': [1,0,0,0,0,0], 'b': [1,1,0,0,0,0], 'c': [1,0,0,1,0,0],
            'd': [1,0,0,1,1,0], 'e': [1,0,0,0,1,0], 'f': [1,1,0,1,0,0],
            'g': [1,1,0,1,1,0], 'h': [1,1,0,0,1,0], 'i': [0,1,0,1,0,0],
            'j': [0,1,0,1,1,0], 'k': [1,0,1,0,0,0], 'l': [1,1,1,0,0,0],
            'm': [1,0,1,1,0,0], 'n': [1,0,1,1,1,0], 'o': [1,0,1,0,1,0],
            'p': [1,1,1,1,0,0], 'q': [1,1,1,1,1,0], 'r': [1,1,1,0,1,0],
            's': [0,1,1,1,0,0], 't': [0,1,1,1,1,0], 'u': [1,0,1,0,0,1],
            'v': [1,1,1,0,0,1], 'w': [0,1,0,1,1,1], 'x': [1,0,1,1,0,1],
            'y': [1,0,1,1,1,1], 'z': [1,0,1,0,1,1],
            # Numbers
            '1': [1,0,0,0,0,0], '2': [1,1,0,0,0,0], '3': [1,0,0,1,0,0],
            '4': [1,0,0,1,1,0], '5': [1,0,0,0,1,0], '6': [1,1,0,1,0,0],
            '7': [1,1,0,1,1,0], '8': [1,1,0,0,1,0], '9': [0,1,0,1,0,0],
            '0': [0,1,0,1,1,0],
            # Space
            ' ': [0,0,0,0,0,0]
        }
        return patterns
    
    def convert_text_to_braille(self, text):
        """Convert text to braille array with word separators"""
        text = text.lower()
        braille_array = []
        in_word = False
        
        for char in text:
            if char in self.braille_map:
                if char == ' ':
                    # Space marks end of word
                    if in_word:
                        braille_array.append([-1, -1, -1, -1, -1, -1])
                        in_word = False
                    braille_array.append(self.braille_map[char])
                else:
                    # Regular character
                    braille_array.append(self.braille_map[char])
                    in_word = True
            else:
                print(f"Warning: Unknown character '{char}' skipped")
        
        # Add separator after last word if text doesn't end with space
        if in_word:
            braille_array.append([-1, -1, -1, -1, -1, -1])
        
        return braille_array
    
    def reshape_braille_array(self, braille_array):
        """
        Reshape the array so that:
        - First 3 rows become column i
        - Next 3 rows become column i+1
        Result: 3 rows x (2 * number of characters) columns
        """
        # Convert to numpy array and transpose
        arr = np.transpose(braille_array)
        
        # arr is now 6 rows x n columns
        # Split into top 3 rows and bottom 3 rows
        top_half = arr[:3, :]      # rows 0,1,2 (dots 1,2,3)
        bottom_half = arr[3:, :]   # rows 3,4,5 (dots 4,5,6)
        
        # Interleave them: column i from top, then column i from bottom
        reshaped = np.zeros((3, arr.shape[1] * 2), dtype=int)
        
        for i in range(arr.shape[1]):
            reshaped[:, 2*i] = top_half[:, i]       # dots 1,2,3
            reshaped[:, 2*i + 1] = bottom_half[:, i]  # dots 4,5,6
        
        return reshaped
    
    def remove_duplicate_separator_columns(self, reshaped_array):
        """
        Remove one column when two consecutive columns are full of -1s
        """
        columns_to_keep = []
        i = 0
        
        while i < reshaped_array.shape[1]:
            # Check if current column and next column are both full of -1s
            if i < reshaped_array.shape[1] - 1:
                current_col = reshaped_array[:, i]
                next_col = reshaped_array[:, i + 1]
                
                # Check if both columns are all -1s
                if np.all(current_col == -1) and np.all(next_col == -1):
                    # Keep only the first column, skip the second
                    columns_to_keep.append(i)
                    i += 2  # Skip both columns (we already kept one)
                else:
                    columns_to_keep.append(i)
                    i += 1
            else:
                # Last column, just keep it
                columns_to_keep.append(i)
                i += 1
        
        # Create new array with only the columns to keep
        if columns_to_keep:
            cleaned_array = reshaped_array[:, columns_to_keep]
        else:
            cleaned_array = reshaped_array
        
        return cleaned_array
    
    def remove_space_after_separator(self, reshaped_array):
        """
        Remove two consecutive columns full of 0s that appear right after a column full of -1s
        This handles the space character that follows a word separator
        """
        columns_to_keep = []
        i = 0
        
        while i < reshaped_array.shape[1]:
            current_col = reshaped_array[:, i]
            
            # Check if current column is all -1s
            if np.all(current_col == -1):
                columns_to_keep.append(i)
                
                # Check if next two columns exist and are both all 0s
                if (i + 2 < reshaped_array.shape[1] and 
                    np.all(reshaped_array[:, i + 1] == 0) and 
                    np.all(reshaped_array[:, i + 2] == 0)):
                    # Skip the next two columns (both are 0s representing space)
                    i += 3
                else:
                    i += 1
            else:
                columns_to_keep.append(i)
                i += 1
        
        # Create new array with only the columns to keep
        if columns_to_keep:
            cleaned_array = reshaped_array[:, columns_to_keep]
        else:
            cleaned_array = reshaped_array
        
        return cleaned_array
    
    def print_braille_array(self, braille_array):
        """Print the braille array in a readable format"""
        print("\n--- Braille Array Output ---")
        print(f"braille_array = [")
        
        for i, pattern in enumerate(braille_array):
            print(f"    {pattern}", end="")
            if i < len(braille_array) - 1:
                print(",")
            else:
                print()
        
        print("]")
        print(f"Total elements: {len(braille_array)}")
        print("--- End ---\n")
    
    def get_braille_pattern(self, char):
        """Get braille pattern for a single character"""
        char = char.lower()
        return self.braille_map.get(char, [0,0,0,0,0,0])


def main():
    converter = BrailleConverter()
    
    print("Text to Braille Converter")
    print("Enter text to convert (or 'quit' to exit):")
    print()
    
    while True:
        text = input("Enter text: ").strip()
        
        if text.lower() == 'quit':
            print("Goodbye!")
            break
        
        if text:
            braille_array = converter.convert_text_to_braille(text)
            converter.print_braille_array(braille_array)
            
            # Reshape the array
            reshaped_array = converter.reshape_braille_array(braille_array)
            
            print("Reshaped Array (before cleanup):")
            print(reshaped_array)
            print(f"Shape: {reshaped_array.shape}")
            print()
            
            # Remove duplicate separator columns
            cleaned_array = converter.remove_duplicate_separator_columns(reshaped_array)
            
            print("After removing duplicate separators:")
            print(cleaned_array)
            print(f"Shape: {cleaned_array.shape}")
            print()
            
            # Remove space columns after separator
            final_array = converter.remove_space_after_separator(cleaned_array)
            
            print("Final Array (after removing spaces after separators):")
            print(final_array)
            print(f"Shape: {final_array.shape}")
            print()


if __name__ == "__main__":
    main()




# converts words into our encodings