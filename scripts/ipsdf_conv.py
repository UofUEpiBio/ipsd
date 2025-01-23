import os
import sys
import base64

def process_text(input_text, output_dir):
    os.makedirs(output_dir, exist_ok=True)

    markdown_output = []
    image_count = 0

    for line in input_text.splitlines():
        if line.startswith("__ipsd_rich_content_image"):
            try:
                # Parse the image data
                _, image_data = line.split(":", 1)
                mime_type, base64_content = image_data.split(",", 1)
                file_extension = mime_type.split("/")[1].split(';')[0]
                image_count += 1
                output_file = os.path.join(output_dir, f"image_{image_count}.{file_extension}")

                # Decode and save the image
                with open(output_file, "wb") as img_file:
                    img_file.write(base64.b64decode(base64_content))

                # Add Markdown image reference immediately
                image_markdown = f"![Image {image_count}](./{output_file})"
                markdown_output.append(image_markdown)
            except ValueError:
                print(f"Malformed image command: {line}", file=sys.stderr)
        else:
            # Add text directly to Markdown output
            if line.strip():
                markdown_output.append(line.strip())

    # Output Markdown content
    print("\n\n".join(markdown_output))

if __name__ == '__main__':
    input_text = sys.stdin.read()

    output_dir = "images"
    process_text(input_text, output_dir)
